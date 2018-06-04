/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/04 01:38:58 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

const t_cmddef	g_cmddef[] = {
	{ NULL, 0, 0 },
	{ "advance", 7, 7 },
	{ "broadcast", 9, 7 },
	{ "connect_nbr", 11, 0 },
	{ "incantation", 11, 300 },
	{ "inventory", 9, 1 },
	{ "kick", 4, 7 },
	{ "fork", 4, 42 },
	{ "left", 4, 7 },
	{ "put", 3, 7 },
	{ "right", 5, 7 },
	{ "see", 3, 7 },
	{ "take", 4, 7 }
};

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

void	set_cmdtype(t_serv *s, t_buff *buff)
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
				buff->scheduled = s->time + def->delay;
				return ;
			}
			++i;
		}
	}
	stpcpy(buff->resp, "ko\n");
	buff->resp_len = 3;
	buff->type = UNDEFINED;
	buff->scheduled = s->time;
}
