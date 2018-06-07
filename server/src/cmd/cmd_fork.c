/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 20:17:58 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/07 02:02:31 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_fork(t_serv *s, int id)
{
	t_ent	*ent;
	t_buff	*buff;
	t_loc	loc;
	t_uint	eggs;

	ent = ENT(s, id);
	buff = CMD_NEXT(&ent->cmds);
	//++s->conn.capacity;
	//++ent->team->authorized;
	// TODO: Egg manager
	loc.x = ent->loc_x;
	loc.y = ent->loc_y;
	eggs = get_res(s, loc, EGG);
	set_res(s, loc, EGG, eggs + 1);
	buff->resp_len = sprintf(buff->resp, "ok\n");
}

int		precmd_fork(t_serv *s, int id)
{
	t_ent	*ent;
	t_loc	loc;

	ent = ENT(s, id);
	loc.x = ent->loc_x;
	loc.y = ent->loc_y;
	return (get_res(s, loc, EGG) < 15);
}
