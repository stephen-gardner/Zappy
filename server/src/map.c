/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 17:51:05 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/19 07:56:07 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "zappy.h"

void		drop_stones(t_serv *s, t_ent *ent)
{
	t_ull	*loc;
	int		type;
	int		amount;

	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	type = LI;
	while (type < EGG)
	{
		if (ent->inv[type])
		{
			amount = RES_GET(loc, type);
			if ((amount += ent->inv[type]) > RES_MAX)
				amount = RES_MAX;
			modify_resource(loc, type, amount);
		}
		++type;
	}
}

int			modify_resource(t_ull *loc, int type, int diff)
{
	int	n;

	if (!diff)
		return (0);
	n = RES_GET(loc, type) + diff;
	if (n < 0 || n > RES_MAX)
		return (-1);
	RES_SET(loc, type, n);
	return (0);
}

static void	gen_resource(t_serv *s, int x, int y, int type)
{
	static int	rates[] = { 50, 40, 30, 30, 25, 20, 15 };
	int			quantity;

	if ((rand() % 100) + 1 > rates[type])
		return ;
	if ((quantity = (rand() % s->nteams) + 1) > RES_MAX)
		quantity = RES_MAX;
	modify_resource(GET_LOC(s, x, y), type, quantity);
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
			type = FOOD;
			while (type < EGG)
				gen_resource(s, x, y, type++);
			++x;
		}
		++y;
	}
}

void		replenish(t_serv *s, int type, int amount)
{
	t_ull	*loc;

	loc = GET_LOC(s, rand() % s->map.width, rand() % s->map.height);
	modify_resource(loc, type, amount);
}
