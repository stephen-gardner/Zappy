/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 01:47:01 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

const t_cmddef	g_cmddef[] = {
	{ ADVANCE, precmd_void, cmd_advance, ADV_STR, ADV_DEL, 0 },
	{ BROADCAST, precmd_void, cmd_broadcast, BRO_STR, BRO_DEL, 1 },
	{ CONNECT_NBR, precmd_void, cmd_connect_nbr, CON_STR, CON_DEL, 0 },
	{ INCANTATION, precmd_incant, cmd_incant, INC_STR, INC_DEL, 0 },
	{ INVENTORY, precmd_void, cmd_inventory, INV_STR, INV_DEL, 0 },
	{ KICK, precmd_void, cmd_kick, KIC_STR, KIC_DEL, 0 },
	{ FORK, precmd_void, cmd_fork, FOR_STR, FOR_DEL, 0 },
	{ LEFT, precmd_void, cmd_left, LEF_STR, LEF_DEL, 0 },
	{ PUT, precmd_put, cmd_put, PUT_STR, PUT_DEL, 1 },
	{ RIGHT, precmd_void, cmd_right, RIG_STR, RIG_DEL, 0 },
	{ SEE, precmd_void, cmd_see, SEE_STR, SEE_DEL, 0 },
	{ TAKE, precmd_take, cmd_take, TAK_STR, TAK_DEL, 1 }
};

const int		g_cmddef_count = sizeof(g_cmddef) / sizeof(t_cmddef);

const t_cmddef	*get_cmddef(int type)
{
	const t_cmddef	*def;
	int				i;

	i = 0;
	while (i < g_cmddef_count)
	{
		def = &g_cmddef[i++];
		if (type == def->type)
			return (def);
	}
	return (NULL);
}

int				precmd_void(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)s;
	(void)id;
	(void)ent;
	(void)buff;
	return (1);
}

void			process_command(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	const t_cmddef	*def;

	if (!ent->team)
		return (add_player(s, buff->recv, id));
	if (buff->type != UNDEFINED)
	{
		def = get_cmddef(buff->type);
		if (def->dispatch(s, id, ent, buff) == -1)
		{
			if (buff->type == INCANTATION)
				build_message(buff, CURR_LEVEL, ent->level);
			else
				build_message(buff, "ko\n");
		}
	}
	send_response(s, id);
}

void			process_precommand(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	const t_cmddef	*def;

	def = get_cmddef(buff->type);
	if (def->pre(s, id, ent, buff))
		buff->scheduled = s->time + def->delay;
	else
	{
		if (buff->type == INCANTATION)
			build_message(buff, CURR_LEVEL, ent->level);
		else
			build_message(buff, "ko\n");
		buff->type = UNDEFINED;
	}
	buff->pre = 1;
}
