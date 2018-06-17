/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/03 00:34:00 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/17 06:35:57 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdio.h>
#include "zappy.h"

void		info(t_serv *s, char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	printf("[%06ju] ", s->time);
	vprintf(fmt, ap);
	printf("\n");
	va_end(ap);
}

void		level_up(t_serv *s, t_ent *ent)
{
	t_buff	*buff;

	buff = CMD_NEXT(&ent->cmds);
	--ent->team->members[ent->level];
	++ent->team->members[++ent->level];
	if (ent->team->members[8] >= 6)
	{
		s->go = 0;
		info(s, "[%s] wins the game!", ent->team->name);
	}
	buff->resp_len = sprintf(buff->resp, "current level : %d\n", ent->level);
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
