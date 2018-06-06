/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/06 02:18:43 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

const t_cmddef	g_cmddef[] = {
	{ UNDEFINED, NULL, NULL, 0, 0 },
	{ ADVANCE, "advance", NULL, 7, 7 },
	{ BROADCAST, "broadcast", NULL, 9, 7 },
	{ CONNECT_NBR, "connect_nbr", cmd_connect_nbr, 11, 0 },
	{ INCANTATION, "incantation", NULL, 11, 300 },
	{ INVENTORY, "inventory", cmd_inventory, 9, 1 },
	{ KICK, "kick", NULL, 4, 7 },
	{ FORK, "fork", cmd_fork, 4, 42 },
	{ LEFT, "left", cmd_left, 4, 7 },
	{ PUT, "put", NULL, 3, 7 },
	{ RIGHT, "right", cmd_right, 5, 7 },
	{ SEE, "see", NULL, 3, 7 },
	{ TAKE, "take", NULL, 4, 7 }
};

void	process_command(t_serv *s, int id)
{
	t_ent			*ent;
	t_buff			*buff;
	const t_cmddef	*def;
	int				i;

	ent = ENT(s, id);
	buff = &ent->cmds.buffs[ent->cmds.start];
	if (!ent->team)
		return (add_player(s, buff->recv, id));
	i = 1;
	while (i < NCOMMANDS)
	{
		def = &g_cmddef[i++];
		if (buff->type == def->type)
			def->dispatch(s, id);
	}
	send_response(s, id);
}

void	set_cmdtype(t_buff *buff)
{
	const t_cmddef	*def;
	int				i;

	if (buff->recv_len < CMD_MAX_LEN)
	{
		i = 1;
		while (i < NCOMMANDS)
		{
			def = &g_cmddef[i];
			if (!strncmp(buff->recv, def->label, def->len))
			{
				buff->type = i;
				buff->scheduled += def->delay;
				return ;
			}
			++i;
		}
	}
	stpcpy(buff->resp, "ko\n");
	buff->resp_len = 3;
	buff->type = UNDEFINED;
}
