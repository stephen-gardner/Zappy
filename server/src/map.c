/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 17:51:05 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/13 17:26:58 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "zappy.h"

const int	g_resrate[] = { 50, 40, 30, 30, 25, 20, 15 };

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
