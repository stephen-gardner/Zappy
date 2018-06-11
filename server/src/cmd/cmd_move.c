/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 16:00:42 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/10 16:07:22 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void		cmd_advance(t_serv *s, int id)
{
	t_ent	*ent;
	t_buff	*buff;
	int		dir;

	ent = ENT(s, id);
	dir = ent->facing;
	if (dir == EAST)
		MOVE(ent->loc_x, s->map.width, 1);
	else if (dir == WEST)
		MOVE(ent->loc_x, s->map.width, -1);
	else if (dir == SOUTH)
		MOVE(ent->loc_y, s->map.height, 1);
	else
		MOVE(ent->loc_y, s->map.height, -1);
	buff = CMD_NEXT(&ent->cmds);
	buff->resp_len = sprintf(buff->resp, "ok\n");
}

static void	turn(t_ent *ent, t_buff *buff, int dir)
{
	ent->facing = ((ent->facing + dir) + 4) % 4;
	buff->resp_len = sprintf(buff->resp, "ok\n");
}

void		cmd_left(t_serv *s, int id)
{
	t_ent	*ent;

	ent = ENT(s, id);
	turn(ent, CMD_NEXT(&ent->cmds), -1);
}

void		cmd_right(t_serv *s, int id)
{
	t_ent	*ent;

	ent = ENT(s, id);
	turn(ent, CMD_NEXT(&ent->cmds), 1);
}
