*This project has been created as part of the 42 curriculum by \<mabar\>*

# Codexion

> **Master the race for resources before the deadline masters you.**

## Description

**Codexion** is a concurrent programming project developed as part of the **42 curriculum**.

The project simulates a group of **coders competing for shared dongles** in order to perform their work. Each coder is represented by a POSIX thread and must acquire two dongles before being allowed to compile.

The main challenge is not simply creating threads. The project focuses on designing a correct synchronization system where multiple threads compete for the same resources without causing:

* Deadlocks
* Race conditions
* Starvation
* Incorrect resource ownership
* Incorrect cooldown handling
* Missed burnout conditions
* Corrupted or interleaved logs

The simulation therefore combines **POSIX threads (`pthread`)**, **mutexes**, **condition variables**, a **priority queue implemented as a binary heap**, and a dedicated **monitor thread**.

### Project goal

The primary goal is to understand how concurrent programs coordinate access to shared resources.

Each coder repeatedly follows a workflow similar to:

```text
Acquire first dongle
        ↓
Try to acquire second dongle
        ↓
If both are available
        ↓
Compile
        ↓
Release dongles
        ↓
Debug
        ↓
Refactor
        ↓
Try again
```

If the second dongle cannot immediately be acquired, the coder must release the first dongle instead of holding it indefinitely.

The monitor continuously checks the state of the simulation and detects when a coder has exceeded its allowed time without compiling.

---

## Features

The implementation contains the following main features:

* One pthread per coder
* Shared dongles protected by mutexes
* Condition variables for blocking and waking waiting coders
* Per-dongle waiting queues
* Priority-based scheduling using a binary min-heap
* EDF-style scheduling support
* Cooldown periods after dongle usage
* Dedicated monitor thread
* Burnout/deadline detection
* Simulation-wide stop state
* Thread-safe logging
* Completion detection
* Graceful waking of blocked threads when the simulation stops

---

# Simulation Model

## Coders

Each coder runs in its own pthread.

A coder has information such as:

* Coder ID
* Number of completed compilations
* Last compilation timestamp
* Left dongle
* Right dongle
* Simulation reference
* Scheduling information

A coder cannot compile unless it owns both required dongles.

## Dongles

A dongle is a shared resource.

Each dongle contains synchronization state such as:

```c
pthread_mutex_t mutex;
pthread_cond_t cond;
```

It also maintains information such as:

* Whether it is currently used
* When its cooldown ends
* Its waiting heap
* The simulation it belongs to

Only one coder can own a dongle at a time.

## Monitor

A dedicated monitor thread periodically checks the simulation.

Its responsibilities include:

1. Checking every coder's last compilation time.
2. Detecting burnout.
3. Detecting when all coders have completed their required work.
4. Updating the global stop state.
5. Waking blocked coders when the simulation ends.

---

# Scheduling

Coders cannot simply compete randomly for dongles.

Each dongle has a waiting queue implemented as a **binary min-heap**.

The heap stores waiting coders using a `t_waiter` structure.

Conceptually:

```text
                 Highest priority
                       ↓
                    Coder A
                  /         \
              Coder B      Coder C
              /    \
          Coder D  Coder E
```

The coder at the root of the heap has the highest priority.

The scheduler can use different priority strategies.

### EDF

With **Earliest Deadline First**, the priority is based on the coder's deadline:

```text
deadline = last_compile_start + time_to_burnout
```

The coder with the earliest deadline gets priority.

This is particularly useful because the simulation has a burnout constraint: a coder that has been waiting for a long time becomes increasingly urgent.

### FIFO-style scheduling

Another scheduling mode can use the order in which coders entered the queue.

This provides a simpler waiting policy while still preventing arbitrary competition between threads.

---

# Blocking cases handled

Concurrency is the central difficulty of Codexion. The implementation addresses several important blocking and synchronization problems.

## 1. Deadlock prevention

A classic deadlock can happen when two coders each hold one dongle and wait forever for the other:

```text
Coder 1:
    owns Dongle A
    waits for Dongle B

Coder 2:
    owns Dongle B
    waits for Dongle A
```

This creates a circular wait.

Instead of allowing a coder to hold its first dongle indefinitely while waiting for its second dongle, the implementation follows the principle:

```text
Acquire first dongle
        ↓
Second dongle unavailable?
        ↓
Release first dongle
        ↓
Wait
        ↓
Try again
```

This prevents the classic circular-wait situation.

### Coffman's conditions

The four Coffman conditions for deadlock are:

1. Mutual exclusion
2. Hold and wait
3. No preemption
4. Circular wait

Dongles naturally satisfy **mutual exclusion** because only one coder can use a dongle at a time.

The implementation primarily attacks **hold-and-wait** and **circular wait**.

A coder does not keep its first dongle while indefinitely waiting for its second dongle.

This significantly reduces the possibility of a circular dependency between coders.

---

## 2. Starvation prevention

A naive implementation using:

```c
pthread_cond_signal()
```

and unrestricted competition can allow one coder to repeatedly acquire a resource while another coder continues waiting.

The waiting heap provides an explicit ordering mechanism.

Instead of:

```text
"Whoever gets scheduled first wins"
```

the scheduler can determine:

```text
"Which waiting coder has priority?"
```

With EDF scheduling, the coder with the closest deadline is prioritized.

This makes the waiting policy more predictable and helps prevent starvation.

---

## 3. Cooldown handling

A dongle does not necessarily become immediately usable after being released.

After a coder finishes compiling, the dongle enters a cooldown period:

```text
Coder uses dongle
       ↓
Compilation
       ↓
Dongle released
       ↓
available_at = current_time + cooldown
       ↓
Dongle unavailable during cooldown
       ↓
Cooldown expires
       ↓
Dongle becomes available
```

The implementation stores the end of the cooldown using an absolute timestamp such as:

```c
dongle->available_at
```

Waiting coders can therefore use a timed condition-variable wait rather than continuously polling with `usleep()`.

---

## 4. Precise burnout detection

The monitor keeps track of each coder's last compilation time.

Conceptually:

```text
current_time - last_compile_time
```

is compared against:

```text
time_to_burnout
```

If the allowed interval is exceeded, the simulation stops and the monitor reports the burnout.

The monitor is therefore independent of the coder threads and can detect a coder that is blocked waiting for resources.

This is important because a coder cannot be responsible for detecting its own burnout while it is blocked inside a condition-variable wait.

---

## 5. Blocked threads during simulation termination

A coder may be sleeping inside:

```c
pthread_cond_wait()
```

when the monitor detects burnout or when all coders finish.

Simply setting:

```c
sim->stop = 1;
```

is not enough.

A thread blocked in `pthread_cond_wait()` may remain blocked.

The simulation therefore needs to wake blocked coders when stopping.

The stop procedure broadcasts to the relevant condition variables so waiting threads can wake up, check the simulation state, and exit.

The general sequence is:

```text
Monitor detects termination
        ↓
Set sim->stop
        ↓
Wake waiting coders
        ↓
Coders return from condition waits
        ↓
Coders check sim->stop
        ↓
Coders terminate
```

---

## 6. Race conditions on the simulation state

The global stop state is shared by multiple threads.

For example:

* The monitor writes it.
* Coder threads read it.
* The main thread may also interact with the simulation lifecycle.

Access is protected by:

```c
pthread_mutex_t stop_mutex;
```

The general pattern is:

```c
pthread_mutex_lock(&sim->stop_mutex);
value = sim->stop;
pthread_mutex_unlock(&sim->stop_mutex);
```

This ensures that reading and writing the stop state does not create a data race.

---

## 7. Race conditions on dongles

A dongle has shared state:

```text
is_used
available_at
waiting heap
```

Multiple coder threads may attempt to access this state simultaneously.

The dongle mutex protects these operations.

For example:

```c
pthread_mutex_lock(&dongle->mutex);

/* inspect and modify dongle state */

pthread_mutex_unlock(&dongle->mutex);
```

This makes acquiring and releasing a dongle an atomic operation from the point of view of competing coder threads.

---

## 8. Log serialization

Multiple coders can print at approximately the same time.

Without synchronization, output such as:

```text
0 1 is compiling
0 2 is debugging
```

could become interleaved or corrupted.

A dedicated print mutex is therefore used to serialize log output.

Conceptually:

```text
Coder 1 ──┐
Coder 2 ──┼──> print mutex ──> stdout
Coder 3 ──┘
```

Only one thread can write a complete log message at a time.

---

# Thread synchronization mechanisms

The project uses POSIX threading primitives provided by `pthread`.

## `pthread_mutex_t`

Mutexes protect shared state from simultaneous modification.

The project uses mutexes for several important resources.

### Dongle mutex

Each dongle has its own mutex:

```c
pthread_mutex_t mutex;
```

This protects:

* `is_used`
* `available_at`
* Waiting-queue operations
* Condition-variable coordination

A coder must own the mutex before inspecting or modifying these fields.

### Stop mutex

The simulation contains a global stop state.

Access is protected by:

```c
pthread_mutex_t stop_mutex;
```

This prevents a coder from reading the stop state while another thread is modifying it.

### Print mutex

Logging uses a dedicated mutex so complete messages are printed atomically.

---

## `pthread_cond_t`

Condition variables allow threads to **sleep until something changes** instead of repeatedly polling.

A condition variable is associated with a mutex.

Typical usage:

```c
pthread_mutex_lock(&dongle->mutex);

while (condition_is_false)
    pthread_cond_wait(&dongle->cond, &dongle->mutex);

pthread_mutex_unlock(&dongle->mutex);
```

The important property of `pthread_cond_wait()` is that it:

1. Releases the mutex.
2. Puts the thread to sleep.
3. Wakes the thread when signaled.
4. Reacquires the mutex before returning.

This makes it suitable for waiting for dongle state changes.

---

## Condition signals

When a dongle becomes available, the thread modifying its state can notify waiting coders:

```c
pthread_cond_signal(&dongle->cond);
```

or:

```c
pthread_cond_broadcast(&dongle->cond);
```

`signal` wakes one waiting thread.

`broadcast` wakes all waiting threads so they can re-check their conditions.

Because several coders may be waiting and because scheduling priority is maintained by the heap, broadcasting can be useful when the state changes and multiple threads need to reconsider whether they can proceed.

---

## Timed condition waits

Cooldowns require a slightly different mechanism.

A coder should not wait indefinitely if a dongle is simply cooling down.

Instead, an absolute deadline can be converted into a `struct timespec` and passed to:

```c
pthread_cond_timedwait()
```

Conceptually:

```text
Dongle used
    ↓
available_at = now + cooldown
    ↓
coder waits
    ↓
condition signal OR timeout
    ↓
re-check dongle state
```

This gives the thread two ways to wake:

1. Another thread changes the dongle state and signals the condition.
2. The cooldown deadline is reached.

The condition must always be re-evaluated after waking because a condition-variable wake-up does not itself guarantee that the resource is available.

---

# Thread-safe communication between coders and the monitor

The monitor and coder threads communicate indirectly through shared synchronized state.

For example:

```text
                  ┌──────────────────┐
                  │     Monitor      │
                  └────────┬─────────┘
                           │
                           │ checks
                           ↓
                 ┌─────────────────────┐
                 │   Shared simulation │
                 │       state         │
                 └─────────────────────┘
                    ↑              ↑
                    │              │
                 reads          writes
                    │              │
             ┌──────┴───┐    ┌─────┴─────┐
             │  Coders   │    │  Monitor  │
             └───────────┘    └───────────┘
```

The monitor does not directly manipulate a coder's execution flow.

Instead, it:

1. Detects a termination condition.
2. Updates the protected stop state.
3. Wakes blocked coders.
4. Coders wake up and observe the new state.
5. Coders terminate safely.

This keeps the communication between threads synchronized and avoids unsafe direct manipulation of another thread's execution.

---

# Race condition example

Consider two coders attempting to acquire the same dongle simultaneously.

Without a mutex:

```text
Coder 1: sees is_used == 0
Coder 2: sees is_used == 0

Coder 1: sets is_used = 1
Coder 2: sets is_used = 1
```

Both threads may believe they acquired the same resource.

With a mutex:

```text
Coder 1:
    lock mutex
    check is_used
    set is_used
    unlock mutex

Coder 2:
    waits for mutex
    lock mutex
    check is_used
    sees it is already used
    unlock mutex
```

Only one coder can successfully acquire the dongle.

---

# Waiting Queue and Heap

Each dongle can have its own waiting heap.

A coder that cannot immediately obtain a resource can be inserted into the corresponding queue.

The heap is implemented as a binary min-heap.

Important operations include:

```c
heap_push()
heap_pop()
heap_top()
heap_empty()
heap_size()
```

The root of the heap represents the coder with the highest scheduling priority.

For EDF scheduling, the priority value is based on the coder's deadline.

This makes resource allocation deterministic according to the selected scheduling policy rather than relying entirely on operating-system thread scheduling.

---

# Cooldown and waiting strategy

The project avoids busy waiting where possible.

Instead of repeatedly doing:

```c
usleep(50);
try_again();
```

the coder can wait on a condition variable.

There are two important events that can wake the coder:

### Resource becomes available

Another coder releases the dongle:

```c
pthread_cond_signal(&dongle->cond);
```

### Cooldown expires

The timed wait reaches:

```text
available_at
```

The coder wakes and checks the resource again.

This approach reduces unnecessary CPU consumption compared with continuously polling.

---

# Simulation termination

The simulation can terminate for two main reasons.

## Burnout

If a coder exceeds its allowed time without compiling:

```text
last compilation
       ↓
time_to_burnout exceeded
       ↓
monitor detects burnout
       ↓
simulation stops
       ↓
waiting coders are awakened
```

The monitor prints the corresponding burnout event.

## All coders finished

When every coder reaches the required number of compilations:

```text
Coder 1 → finished
Coder 2 → finished
Coder 3 → finished
...
Coder N → finished
       ↓
Monitor detects completion
       ↓
simulation stops
```

---

# Compilation

The project is written in C and uses the POSIX threads API.

A typical compilation command is:

```bash
make
```

The exact compiler flags and object files are defined by the project's `Makefile`.

To clean object files:

```bash
make clean
```

To remove all generated build files:

```bash
make fclean
```

To rebuild everything:

```bash
make re
```

---

# Execution

After compilation, run the generated executable according to the project's command-line interface.

For example:

```bash
./codexion
```

If the project requires simulation parameters, they can be supplied as command-line arguments according to the project's argument format.

For example:

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <required_compiles>
```

The exact accepted argument format should be checked in the program's argument parser and subject specification.

---

# Example output

A normal execution may produce output similar to:

```text
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
20 1 is debugging
40 1 is refactoring
40 2 has taken a dongle
40 2 has taken a dongle
40 2 is compiling
```

A burnout can be reported as:

```text
301 1 burned out
```

The timestamp represents the elapsed simulation time.

---

# Project Architecture

The implementation is divided into several logical components.

```text
                    ┌───────────────────┐
                    │       main        │
                    └─────────┬─────────┘
                              │
                              ↓
                    ┌───────────────────┐
                    │    Simulation     │
                    └─────────┬─────────┘
                              │
             ┌────────────────┼────────────────┐
             │                │                │
             ↓                ↓                ↓
        Coder threads      Monitor          Dongles
             │                │                │
             │                │                │
             └────────────────┼────────────────┘
                              │
                              ↓
                    Condition variables
                              │
                              ↓
                         Mutexes
```

### Main components

**Simulation**

Responsible for global state and lifecycle management.

**Coder operations**

Contains resource acquisition, release, cooldown handling, and coder execution.

**Scheduler**

Contains the waiting heap and scheduling logic.

**Monitor**

Detects burnout and simulation completion.

**Logging**

Serializes output between concurrent threads.

---

# Technical choices

## POSIX threads

`pthread` was chosen because the project specifically focuses on multithreading and synchronization.

It provides:

* Thread creation
* Thread joining
* Mutexes
* Condition variables
* Timed condition waits

## Per-dongle synchronization

Instead of using one global mutex for every dongle, each dongle has its own synchronization primitive.

This allows unrelated dongles to be manipulated concurrently.

For example:

```text
Coder A → Dongle 1 mutex
Coder B → Dongle 2 mutex
```

They do not necessarily have to block each other.

## Condition variables instead of polling

Condition variables allow blocked threads to sleep until there is a reason to retry.

This is more appropriate for resource contention than continuously checking a shared variable.

## Binary heap

A binary heap provides efficient priority-queue operations.

Insertion and removal have logarithmic complexity:

```text
push: O(log n)
pop:  O(log n)
top:  O(1)
```

This makes it suitable for maintaining a scheduling queue for each dongle.

---

# Important concurrency principles

The project demonstrates several important rules for multithreaded programming.

### Always protect shared state

If multiple threads access the same mutable variable, synchronization must be considered.

### Always check conditions after waking

A condition variable does not mean:

> "The resource is definitely available."

It means:

> "Something changed; check the condition again."

Therefore:

```c
while (!condition)
    pthread_cond_wait(...);
```

is preferred over:

```c
if (!condition)
    pthread_cond_wait(...);
```

### Do not hold resources unnecessarily

Holding one dongle while waiting indefinitely for another is dangerous because it can create deadlocks.

### Use absolute deadlines for timed waits

Cooldowns can be represented by an absolute timestamp such as:

```c
available_at
```

rather than repeatedly calculating short sleep intervals.

---

# Resources

## POSIX Threads

* `pthread_create()` — creates threads.
* `pthread_join()` — waits for thread termination.
* `pthread_mutex_lock()` / `pthread_mutex_unlock()` — mutex synchronization.
* `pthread_cond_wait()` / `pthread_cond_signal()` / `pthread_cond_broadcast()` — condition-variable synchronization.
* `pthread_cond_timedwait()` — timed condition-variable waiting.

Official POSIX documentation:

[The Open Group — POSIX Threads](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread.h.html?utm_source=chatgpt.com)

## Linux pthread documentation

[Linux man-pages — pthreads](https://man7.org/linux/man-pages/man7/pthreads.7.html?utm_source=chatgpt.com)

[pthread_mutex_lock — Linux man-pages](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html?utm_source=chatgpt.com)

[pthread_cond_wait — Linux man-pages](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3.html?utm_source=chatgpt.com)

[pthread_cond_timedwait — Linux man-pages](https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html?utm_source=chatgpt.com)

## Deadlocks

The project uses the classic Coffman conditions as a conceptual framework for understanding deadlocks:

* Mutual exclusion
* Hold and wait
* No preemption
* Circular wait

Understanding these conditions was essential for designing the dongle acquisition strategy.

## Scheduling

The scheduling design is based on priority queues and, where applicable, **Earliest Deadline First (EDF)** scheduling.

EDF is a dynamic scheduling strategy where the task with the earliest deadline receives the highest priority.

---

# AI usage

AI was used as a **learning and debugging assistant** during development.

It was used for:

* Explaining POSIX thread concepts.
* Understanding how the operating-system scheduler handles many threads.
* Understanding mutexes and condition variables.
* Explaining `pthread_cond_wait()` and `pthread_cond_timedwait()`.
* Investigating possible deadlocks and the Coffman conditions.
* Analyzing race-condition scenarios.
* Reviewing the synchronization strategy.
* Debugging segmentation faults and synchronization problems.
* Understanding how condition variables should be used with mutex-protected predicates.
* Discussing heap-based scheduling and priority queues.
* Reviewing the interaction between coder threads, dongles, and the monitor.
* Improving the project documentation and README structure.

AI was **not used as a replacement for understanding or implementing the concurrency model**. The project code was developed, tested, modified, and debugged as part of the learning process.

Particular attention was paid to understanding the reasoning behind synchronization decisions rather than blindly copying generated solutions.

---

# What this project demonstrates

Codexion provides practical experience with:

* C programming
* POSIX threads
* Concurrent programming
* Mutex synchronization
* Condition variables
* Timed waits
* Race-condition prevention
* Deadlock prevention
* Starvation prevention
* Priority scheduling
* Binary heaps
* Resource management
* Thread-safe logging
* Monitoring concurrent systems
* Deadline/burnout detection
* Shared-state synchronization

The main lesson of the project is that **creating threads is easy; correctly coordinating them is the difficult part**.

---