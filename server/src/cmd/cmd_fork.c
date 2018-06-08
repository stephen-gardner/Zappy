/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 20:17:58 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/08 01:00:07 by sgardner         ###   ########.fr       */
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
	if ((eggs = RES_GET(loc, EGG)) == RES_MAX)
		return ;
	RES_SET(loc, EGG, eggs + 1);
	buff->resp_len = sprintf(buff->resp, "ok\n");
}
