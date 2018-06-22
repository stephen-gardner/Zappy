/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 16:00:42 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 00:28:58 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

int			cmd_advance(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)id;
	move_dir(s, ent, ent->facing);
	OK(buff);
	return (0);
}

int			cmd_left(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)s;
	(void)id;
	CHDIR(ent->facing, 2);
	OK(buff);
	return (0);
}

int			cmd_right(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)s;
	(void)id;
	CHDIR(ent->facing, -2);
	OK(buff);
	return (0);
}

void		move_dir(t_serv *s, t_ent *ent, int dir)
{
	if (dir == EAST)
		MOVE(ent->loc_x, s->map.width, 1);
	else if (dir == WEST)
		MOVE(ent->loc_x, s->map.width, -1);
	else if (dir == SOUTH)
		MOVE(ent->loc_y, s->map.height, 1);
	else
		MOVE(ent->loc_y, s->map.height, -1);
}
