/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 05:14:36 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

const t_evdef	g_evdef[] = {
	{ EV_ADV, ENT_PLAYER, preev_void, ev_advance, ADV_STR, ADV_DEL, 0 },
	{ EV_BRO, ENT_PLAYER, preev_void, ev_broadcast, BRO_STR, BRO_DEL, 1 },
	{ EV_CON, ENT_PLAYER, preev_void, ev_connect_nbr, CON_STR, CON_DEL, 0 },
	{ EV_FOR, ENT_PLAYER, preev_void, ev_fork, FOR_STR, FOR_DEL, 0 },
	{ EV_INC, ENT_PLAYER, preev_incant, ev_incant, INC_STR, INC_DEL, 0 },
	{ EV_INV, ENT_PLAYER, preev_void, ev_inventory, INV_STR, INV_DEL, 0 },
	{ EV_KIC, ENT_PLAYER, preev_void, ev_kick, KIC_STR, KIC_DEL, 0 },
	{ EV_LEF, ENT_PLAYER, preev_void, ev_left, LEF_STR, LEF_DEL, 0 },
	{ EV_PUT, ENT_PLAYER, preev_put, ev_put, PUT_STR, PUT_DEL, 1 },
	{ EV_RIG, ENT_PLAYER, preev_void, ev_right, RIG_STR, RIG_DEL, 0 },
	{ EV_SEE, ENT_PLAYER, preev_void, ev_see, SEE_STR, SEE_DEL, 0 },
	{ EV_TAK, ENT_PLAYER, preev_take, ev_take, TAK_STR, TAK_DEL, 1 },
	{ EV_MSZ, ENT_GRAPHIC, preev_void, ev_msz, MSZ_STR, MSZ_DEL, 0 },
	{ EV_SGT, ENT_GRAPHIC, preev_void, ev_sgt, SGT_STR, SGT_DEL, 0 },
	{ EV_SST, ENT_GRAPHIC, preev_void, ev_sst, SST_STR, SST_DEL, 1 },
	{ EV_TNA, ENT_GRAPHIC, preev_void, ev_tna, TNA_STR, TNA_DEL, 0 }
};

const int		g_evdef_count = sizeof(g_evdef) / sizeof(t_evdef);

const t_evdef	*get_evdef(int enttype, int evtype)
{
	const t_evdef	*def;
	int				i;

	i = 0;
	while (i < g_evdef_count)
	{
		def = &g_evdef[i++];
		if (enttype == def->enttype && evtype == def->evtype)
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

	if (ent->type == ENT_UNDEFINED)
	{
		if (!strcmp(AUTH_GRAPHIC, buff->data))
			add_gfx_client(s, id, ent);
		else
			add_player(s, id, ent, buff);
		return ;
	}
	if (buff->type != UNDEFINED)
	{
		def = get_evdef(ent->type, buff->type);
		if (def->dispatch(s, id, ent, buff) == -1)
		{
			if (buff->type == EV_INC)
				build_message(s, CURR_LEVEL, ent->level);
			else
				(ent->type == ENT_PLAYER) ? KO(s) : build_message(s, "sbp\n");
		}
	}
	send_response(s, id);
}

void			process_precommand(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	const t_evdef	*def;

	def = get_evdef(ent->type, buff->type);
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
