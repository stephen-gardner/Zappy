/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/03 00:34:00 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/06 01:25:35 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdio.h>
#include "zappy.h"

void		info(t_serv *s, char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	printf("[%0.5ju] ", s->time);
	vprintf(fmt, ap);
	printf("\n");
	va_end(ap);
}

t_timespec	time_diff(t_timespec t1, t_timespec t2)
{
	t1.tv_sec -= t2.tv_sec;
	if ((t1.tv_nsec -= t2.tv_nsec) < 0)
	{
		--t1.tv_sec;
		t1.tv_nsec += 1000000000;
	}
	return (t1);
}
