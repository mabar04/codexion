/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:15 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 20:04:44 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_putnbr(char s)
{
	int	a;

	a = s - '0';
	return (a);
}

int	check_valid(char *s)
{
	if (!s)
		return (0);
	while (*s >= 9 && *s <= 13)
		s++;
	if (*s == '+')
		s++;
	if (*s == '\0')
		return (0);
	while (*s >= '0' && *s <= '9')
		s++;
	if (*s == '\0')
		return (1);
	return (0);
}

long	valid_long(char *s)
{
	unsigned long	result;
	unsigned long	digit;

	result = 0;
	if (!check_valid(s))
		return (-1);
	while (*s >= 9 && *s <= 13)
		s++;
	if (*s == '+')
		s++;
	while (*s)
	{
		digit = ft_putnbr(*s);
		if (result > ((unsigned long)LONG_MAX - digit) / 10)
			return (-1);
		result = result * 10 + digit;
		s++;
	}
	return ((long)result);
}

int	valid_int(char *s)
{
	long	a;

	a = valid_long(s);
	if (a == -1 || a > INT_MAX)
		return (-1);
	return ((int)a);
}

int	check_scheduler(char *s)
{
	if (!strcmp(s, "fifo") || !strcmp(s, "edf"))
		return (1);
	return (0);
}
