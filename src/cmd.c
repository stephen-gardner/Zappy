/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/29 20:26:32 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	queue_commands(t_serv *s, int id)
{
	t_ent	*ent;
	t_buff	*rbuff;
	char	*cmd;

	ent = &s->conn.ents[id];
	rbuff = &ent->rbuff;
	if (!rbuff->ncmds)
		return ;
	cmd = rbuff->data[CMD_POS(rbuff, 0)];
	if (!ent->team && add_player(s, cmd, id) < 0)
		remove_socket(&s->conn, id);
}
