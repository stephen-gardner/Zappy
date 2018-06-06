/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_turn.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 16:00:42 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/05 16:15:03 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

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
