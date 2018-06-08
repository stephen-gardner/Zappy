/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 20:17:58 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/07 19:05:57 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_fork(t_serv *s, int id)
{
	t_ent	*ent;
	t_buff	*buff;
	t_uint	*loc;
	t_uint	eggs;

	ent = ENT(s, id);
	buff = CMD_NEXT(&ent->cmds);
	//++s->conn.capacity;
	//++ent->team->authorized;
	// TODO: Egg manager
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	eggs = GET_RES(loc, EGG);
	SET_RES(loc, EGG, eggs + 1);
	buff->resp_len = sprintf(buff->resp, "ok\n");
}

int		precmd_fork(t_serv *s, int id)
{
	t_ent	*ent;
	t_uint	*loc;

	ent = ENT(s, id);
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	return (GET_RES(loc, EGG) < 15);
}
