/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_sst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 02:59:10 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 03:51:53 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "zappy.h"

static int	parse_tickrate(char *arg)
{
	char	*endptr;
	long	ticks;

	ticks = strtol(arg, &endptr, 10);
	if (*endptr || ticks < 1 || ticks > INT_MAX)
		return (-1);
	return ((int)ticks);
}

int			ev_sst(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	int	ticks;

	if ((ticks = parse_tickrate(strchr(buff->data, ' ') + 1)) == -1)
		return (-1);
	set_tickrate(s, ticks);
	ev_sgt(s, id, ent, buff);
	info(s, "* Server tickrate set to %d", ticks);
	return (0);
}

void		set_tickrate(t_serv *s, int ticks)
{
	s->ticks = ticks;
	if (ticks > 1)
	{
		s->tickrate.tv_sec = 0;
		s->tickrate.tv_nsec = 1000000000 / ticks;
	}
	else
	{
		s->tickrate.tv_sec = 1;
		s->tickrate.tv_nsec = 0;
	}
}
