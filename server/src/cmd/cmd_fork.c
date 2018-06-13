/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 20:17:58 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/13 00:06:56 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_fork(t_serv *s, int id)
{
	t_ent	*ent;
	t_buff	*buff;
	t_uint	*loc;

	ent = ENT(s, id);
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	if (modify_resource(loc, EGG, 1) < 0)
		return ;
	add_egg(s, ent->team, ent->loc_x, ent->loc_y);
	buff = CMD_NEXT(&ent->cmds);
	buff->resp_len = sprintf(buff->resp, "ok\n");
}
