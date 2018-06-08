/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/08 02:25:45 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

const t_cmddef	g_cmddef[] = {
	{ ADVANCE, precmd_void, cmd_advance, "advance", 7, 7, 0 },
	{ BROADCAST, precmd_void, NULL, "broadcast", 9, 7, 1 },
	{ CONNECT_NBR, precmd_void, cmd_connect_nbr, "connect_nbr", 11, 0, 0 },
	{ INCANTATION, NULL, NULL, "incantation", 11, 300, 0 },
	{ INVENTORY, precmd_void, cmd_inventory, "inventory", 9, 1, 0 },
	{ KICK, precmd_void, NULL, "kick", 4, 7, 0 },
	{ FORK, precmd_void, cmd_fork, "fork", 4, 42, 0 },
	{ LEFT, precmd_void, cmd_left, "left", 4, 7, 0 },
	{ PUT, precmd_put, cmd_put, "put", 3, 7, 1 },
	{ RIGHT, precmd_void, cmd_right, "right", 5, 7, 0 },
	{ SEE, precmd_void, NULL, "see", 3, 7, 0 },
	{ TAKE, precmd_take, cmd_take, "take", 4, 7, 1 }
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
	buff = CMD_NEXT(&ent->cmds);
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

static int		cmdcmp(char *cmd, const t_cmddef *def)
{
	if (strncmp(cmd, def->label, def->len))
		return (1);
	if (def->args)
		return (*(cmd + def->len) != ' ');
	else
		return (*(cmd + def->len) != '\0');
}

// TODO: Move precmd execution to where command is at top of queue

void			set_cmdtype(t_serv *s, int id)
{
	t_cmd			*cmds;
	t_buff			*buff;
	const t_cmddef	*def;
	int				i;

	cmds = GET_CMDS(s, id);
	buff = &cmds->buffs[CMD_POS(cmds, cmds->ncmds)];
	stpcpy(buff->resp, "ko\n");
	buff->resp_len = 3;
	buff->type = UNDEFINED;
	if (buff->recv_len < CMD_MAX_LEN)
	{
		i = 0;
		while (i < g_cmddef_count)
		{
			def = &g_cmddef[i++];
			if (!cmdcmp(buff->recv, def) && def->pre(s, id))
			{
				buff->type = def->type;
				buff->scheduled += def->delay;
				return ;
			}
		}
	}
}
