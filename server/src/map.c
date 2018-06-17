/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 17:51:05 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/14 14:58:54 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "zappy.h"

const int	g_resrate[] = { 50, 40, 30, 30, 25, 20, 15 };

void		drop_stones(t_serv *s, t_ent *ent)
{
	t_uint	*loc;
	int		amount;
	int		i;

	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	i = 1;
	while (i < EGG)
	{
		if (ent->inv[i])
		{
			amount = RES_GET(loc, i);
			if ((amount += ent->inv[i]) > RES_MAX)
				amount = RES_MAX;
			modify_resource(loc, i, amount);
		}
		++i;
	}
}

int			modify_resource(t_uint *loc, int type, int diff)
{
	int	n;

	if (!diff)
		return (0);
	n = RES_GET(loc, type);
	n += diff;
	if (n < 0 || n > RES_MAX)
		return (-1);
	RES_SET(loc, type, n);
	return (0);
}

static void	gen_resource(t_serv *s, int x, int y, int type)
{
	t_uint	*loc;
	int		chance;

	chance = (rand() % 100) + 1;
	if (chance > g_resrate[type])
		return ;
	loc = GET_LOC(s, x, y);
	modify_resource(loc, type, (rand() % s->nteams) + 1);
}

void		populate_map(t_serv *s)
{
	int		x;
	int		y;
	int		type;

	printf("Generating resources...\n");
	y = 0;
	while (y < s->map.height)
	{
		x = 0;
		while (x < s->map.width)
		{
			type = 0;
			while (type < EGG)
				gen_resource(s, x, y, type++);
			++x;
		}
		++y;
	}
}

void		replenish(t_serv *s, int type, int amount)
{
	t_uint	*loc;

	loc = GET_LOC(s, rand() % s->map.width, rand() % s->map.height);
	modify_resource(loc, type, amount);
}
