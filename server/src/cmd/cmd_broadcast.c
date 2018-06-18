/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_broadcast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/14 19:59:26 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/17 17:59:55 by sgardner         ###   ########.fr       */
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
	if (MAX(pent->loc_x, cent->loc_x) == pent->loc_x)
		rdx *= -1;
	rdy = s->map.height - MAX(pent->loc_y, cent->loc_y)
		+ MIN(pent->loc_y, cent->loc_y);
	if (MAX(pent->loc_y, cent->loc_y) == pent->loc_y)
		rdy *= -1;
	vec[0] = (ABS(dx) < ABS(rdx)) ? dx : rdx;
	vec[1] = (ABS(dy) < ABS(rdy)) ? dy : rdy;
}

static int	get_dir(t_serv *s, t_ent *pent, t_ent *cent)
{
	int	vec[2];
	int	dir;

	if (pent == cent)
		return (0);
	calc_vector(s, pent, cent, vec);
	dir = (vec[1] < 0) ? NORTH : SOUTH;
	if (vec[1])
	{
		if (dir == NORTH)
			CHDIR(dir, (vec[0] > 0) ? -1 : 1);
		else
			CHDIR(dir, (vec[0] > 0) ? 1 : -1);
		if (ABS(vec[1]) != ABS(vec[0]))
		{
			if (dir == NWEST || dir == SEAST)
				CHDIR(dir, (ABS(vec[0]) > ABS(vec[1])) ? 1 : -1);
			else
				CHDIR(dir, (ABS(vec[0]) > ABS(vec[1])) ? -1 : 1);
		}
	}
	else
		dir = (vec[0] > 0) ? EAST : WEST;
	CHDIR(dir, cent->facing);
	return (dir + 1);
}

void		cmd_broadcast(t_serv *s, int id)
{
	t_ent	*pent;
	t_buff	*buff;
	char	*msg;
	int		k;
	int		i;

	pent = ENT(s, id);
	buff = CMD_NEXT(&pent->cmds);
	msg = strchr(buff->recv, ' ') + 1;
	i = 1;
	while (i < s->conn.nsockets)
	{
		k = get_dir(s, pent, ENT(s, i));
		dprintf(SOCK(s, i), "message %d,%s\n", k, msg);
		++i;
	}
	buff->resp_len = sprintf(buff->resp, "ok\n");
}
