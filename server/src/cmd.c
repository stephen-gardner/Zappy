/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/03 19:14:18 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	process_command(t_serv *s, int id)
{
	t_ent	*ent;
	t_buff	*buff;

	ent = ENT(s, id);
	buff = &ent->cmds.buffs[ent->cmds.start];
	if (!ent->team)
		return (add_player(s, buff->recv, id));
	else if (buff->type == CONNECT_NBR)
		cmd_connect_nbr(s, ent->team, id, 0);
	send_response(s, id);
}
