/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/07 01:58:42 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

const t_cmddef	g_cmddef[] = {
	{ ADVANCE, precmd_void, cmd_advance, "advance", 7, 7 },
	{ BROADCAST, precmd_void, NULL, "broadcast", 9, 7 },
	{ CONNECT_NBR, precmd_void, cmd_connect_nbr, "connect_nbr", 11, 0 },
	{ INCANTATION, NULL, NULL, "incantation", 11, 300 },
	{ INVENTORY, precmd_void, cmd_inventory, "inventory", 9, 1 },
	{ KICK, precmd_void, NULL, "kick", 4, 7 },
	{ FORK, precmd_fork, cmd_fork, "fork", 4, 42 },
	{ LEFT, precmd_void, cmd_left, "left", 4, 7 },
	{ PUT, NULL, NULL, "put", 3, 7 },
	{ RIGHT, precmd_void, cmd_right, "right", 5, 7 },
	{ SEE, precmd_void, NULL, "see", 3, 7 },
	{ TAKE, NULL, NULL, "take", 4, 7 }
};

const int		g_cmddef_count = sizeof(g_cmddef) / sizeof(t_cmddef);

int				precmd_void(t_serv *s, int id)
{
	(void)s;
	(void)id;
	return (1);
}

void			process_command(t_serv *s, int id)
{
	t_ent			*ent;
	t_buff			*buff;
	const t_cmddef	*def;
	int				i;

	ent = ENT(s, id);
	buff = &ent->cmds.buffs[ent->cmds.start];
	if (!ent->team)
		return (add_player(s, buff->recv, id));
	i = 0;
	while (i < g_cmddef_count)
	{
		def = &g_cmddef[i++];
		if (buff->type == def->type)
			def->dispatch(s, id);
	}
	send_response(s, id);
}

void			set_cmdtype(t_serv *s, int id)
{
	t_buff			*buff;
	const t_cmddef	*def;
	int				i;

	buff = CMD_NEXT(GET_CMDS(s, id));
	if (buff->recv_len < CMD_MAX_LEN)
	{
		i = 0;
		while (i < g_cmddef_count)
		{
			def = &g_cmddef[i++];
			if (!strncmp(buff->recv, def->label, def->len) && def->pre(s, id))
			{
				buff->type = def->type;
				buff->scheduled += def->delay;
				return ;
			}
		}
	}
	stpcpy(buff->resp, "ko\n");
	buff->resp_len = 3;
	buff->type = UNDEFINED;
}
