/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 22:42:56 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

const t_evdef	g_evdef[] = {
	{ EV_ADV, preev_void, ev_advance, ADV_STR, ADV_DEL, 0 },
	{ EV_BRO, preev_void, ev_broadcast, BRO_STR, BRO_DEL, 1 },
	{ EV_CON, preev_void, ev_connect_nbr, CON_STR, CON_DEL, 0 },
	{ EV_INC, preev_incant, ev_incant, INC_STR, INC_DEL, 0 },
	{ EV_INV, preev_void, ev_inventory, INV_STR, INV_DEL, 0 },
	{ EV_KIC, preev_void, ev_kick, KIC_STR, KIC_DEL, 0 },
	{ EV_FOR, preev_void, ev_fork, FOR_STR, FOR_DEL, 0 },
	{ EV_LEF, preev_void, ev_left, LEF_STR, LEF_DEL, 0 },
	{ EV_PUT, preev_put, ev_put, PUT_STR, PUT_DEL, 1 },
	{ EV_RIG, preev_void, ev_right, RIG_STR, RIG_DEL, 0 },
	{ EV_SEE, preev_void, ev_see, SEE_STR, SEE_DEL, 0 },
	{ EV_TAK, preev_take, ev_take, TAK_STR, TAK_DEL, 1 }
};

const int		g_evdef_count = sizeof(g_evdef) / sizeof(t_evdef);

const t_evdef	*get_evdef(int type)
{
	const t_evdef	*def;
	int				i;

	i = 0;
	while (i < g_evdef_count)
	{
		def = &g_evdef[i++];
		if (type == def->type)
			return (def);
	}
	return (NULL);
}

int				preev_void(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)s;
	(void)id;
	(void)ent;
	(void)buff;
	return (0);
}

void			process_command(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	const t_evdef	*def;

	if (!ent->team)
		return (add_player(s, id, ent, buff));
	if (buff->type != UNDEFINED)
	{
		def = get_evdef(buff->type);
		if (def->dispatch(s, id, ent, buff) == -1)
		{
			if (buff->type == EV_INC)
				build_message(s, CURR_LEVEL, ent->level);
			else
				KO(s);
		}
	}
	send_response(s, id);
}

void			process_precommand(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	const t_evdef	*def;

	def = get_evdef(buff->type);
	if (def->pre(s, id, ent, buff) == -1)
	{
		if (buff->type == EV_INC)
			build_message(s, CURR_LEVEL, ent->level);
		else
			KO(s);
		buff->type = UNDEFINED;
		buff->scheduled = s->time;
	}
	else
		buff->scheduled = s->time + def->delay;
	buff->pre = 1;
}
