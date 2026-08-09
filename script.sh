#!/bin/bash

for file in *.c; do
    echo "Formatting and running Std on $file..."
    python3 -m c_formatter_42 "$file"
    vim -c "Std" -c "wq" "$file"
done