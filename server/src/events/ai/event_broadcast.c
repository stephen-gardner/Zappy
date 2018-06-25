/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_broadcast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/14 19:59:26 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/24 14:40:18 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

#define ABS(x)		(((x) < 0) ? -(x) : (x))
#define MIN(x, y)	(x < y ? x : y)
#define MAX(x, y)	(x > y ? x : y)

static void	calc_vector(t_serv *s, t_ent *pent, t_ent *cent, int *vec)
{
	int	dx;
	int	dy;
	int	rdx;
	int	rdy;

	dx = pent->loc_x - cent->loc_x;
	dy = pent->loc_y - cent->loc_y;
	rdx = s->map.width - MAX(pent->loc_x, cent->loc_x)
		+ MIN(pent->loc_x, cent->loc_x);
	if (pent->loc_x > cent->loc_x)
		rdx *= -1;
	rdy = s->map.height - MAX(pent->loc_y, cent->loc_y)
		+ MIN(pent->loc_y, cent->loc_y);
	if (pent->loc_y > cent->loc_y)
		rdy *= -1;
	vec[0] = (ABS(dx) < ABS(rdx)) ? dx : rdx;
	vec[1] = (ABS(dy) < ABS(rdy)) ? dy : rdy;
	vec[2] = ABS(vec[0]);
	vec[3] = ABS(vec[1]);
}

static int	get_dir(t_serv *s, t_ent *pent, t_ent *cent)
{
	int	vec[4];
	int	dir;

	if (pent->loc_x == cent->loc_x && pent->loc_y == cent->loc_y)
		return (0);
	calc_vector(s, pent, cent, vec);
	dir = (vec[1] < 0) ? NORTH : SOUTH;
	if (vec[1])
	{
		if (dir == NORTH)
			CHDIR(dir, (vec[0] > 0) ? -1 : 1);
		else
			CHDIR(dir, (vec[0] > 0) ? 1 : -1);
		if (vec[2] != vec[3])
		{
			if (dir == NWEST || dir == SEAST)
				CHDIR(dir, (vec[2] > vec[3]) ? 1 : -1);
			else
				CHDIR(dir, (vec[2] > vec[3]) ? -1 : 1);
		}
	}
	else
		dir = (vec[0] > 0) ? EAST : WEST;
	CHDIR(dir, -cent->facing);
	return (dir + 1);
}

int			ev_broadcast(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_ent	*cent;
	char	*msg;
	int		k;
	int		i;

	(void)id;
	msg = strchr(buff->data, ' ') + 1;
	i = 1;
	while (i < s->conn.nsockets)
	{
		cent = ENT(s, i);
		if (cent->type == ENT_PLAYER)
		{
			k = get_dir(s, ent, cent);
			dprintf(SOCK(s, i), "message %d,%s\n", k, msg);
		}
		++i;
	}
	OK(s);
	return (0);
}
