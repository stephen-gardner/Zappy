/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_incantation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 02:29:34 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/23 18:06:15 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

#define LVI(lv)	(((lv > g_elevreq_count) ? g_elevreq_count : lv) - 1)

const t_elevreq	g_elevreq[] = {
	{ 1, { 0, 1, 0, 0, 0, 0, 0, 0 } },
	{ 2, { 0, 1, 1, 1, 0, 0, 0, 0 } },
	{ 2, { 0, 2, 0, 1, 0, 2, 0, 0 } },
	{ 4, { 0, 1, 1, 2, 0, 1, 0, 0 } },
	{ 4, { 0, 1, 2, 1, 3, 0, 0, 0 } },
	{ 6, { 0, 1, 2, 3, 0, 1, 0, 0 } },
	{ 6, { 0, 2, 2, 2, 2, 2, 1, 0 } }
};

const int		g_elevreq_count = sizeof(g_elevreq) / sizeof(t_elevreq);

static int		is_ready(t_ent *ent, t_ent *cent, int finished)
{
	t_buff	*buff;
	t_buff	*cbuff;

	if (cent == ent
		|| cent->level != ent->level
		|| cent->loc_x != ent->loc_x
		|| cent->loc_y != ent->loc_y)
		return (0);
	if (finished)
	{
		buff = EV_NEXT(&ent->evs);
		cbuff = EV_NEXT(&cent->evs);
		if (!cent->evs.nevs
			|| cbuff->type != EV_INC
			|| cbuff->scheduled != buff->scheduled)
			return (0);
	}
	else if (cent->evs.nevs)
		return (0);
	return (1);
}

static int		count_ready(t_serv *s, t_ent *ent, int finished)
{
	t_ent	*cent;
	int		count;
	int		i;

	i = 1;
	count = 1;
	while (i < s->conn.nsockets)
	{
		cent = ENT(s, i);
		if (is_ready(ent, cent, finished))
			++count;
		++i;
	}
	return (count);
}

int				ev_incant(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	const t_elevreq	*req;
	t_ent			*cent;
	int				count;
	int				i;

	(void)id;
	(void)buff;
	req = &g_elevreq[LVI(ent->level)];
	if (count_ready(s, ent, 1) < req->nplayers)
		return (-1);
	i = 1;
	count = 1;
	while (count < req->nplayers && i < s->conn.nsockets)
	{
		cent = ENT(s, i);
		if (cent != ent && is_ready(ent, cent, 1))
		{
			level_up(s, cent);
			send_response(s, i);
			++count;
		}
		++i;
	}
	level_up(s, ent);
	return (0);
}

static void		begin_incant(t_serv *s, t_ent *ent)
{
	const t_elevreq	*req;
	t_ent			*cent;
	t_buff			*cbuff;
	int				count;
	int				i;

	i = 1;
	count = 1;
	req = &g_elevreq[LVI(ent->level)];
	while (count < req->nplayers && i < s->conn.nsockets)
	{
		cent = ENT(s, i);
		if (is_ready(ent, cent, 0))
		{
			++cent->evs.nevs;
			cbuff = EV_NEXT(&cent->evs);
			memcpy(cbuff, EV_NEXT(&ent->evs), sizeof(t_buff));
			cbuff->pre = 1;
			cbuff->scheduled = s->time + get_evdef(EV_INC)->delay;
			dprintf(SOCK(s, i), ELEVATING);
			++count;
		}
		++i;
	}
}

int				preev_incant(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	const t_elevreq	*req;
	t_ull			*loc;
	int				i;

	(void)buff;
	if (ent->level >= MAX_LEVEL)
		return (-1);
	req = &g_elevreq[LVI(ent->level)];
	if (count_ready(s, ent, 0) < req->nplayers)
		return (-1);
	i = 0;
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	while (i < NRES)
	{
		if (RES_GET(loc, i) < req->nitems[i])
			return (-1);
		++i;
	}
	i = -1;
	while (++i < NRES)
		modify_resource(loc, i, -req->nitems[i]);
	begin_incant(s, ent);
	dprintf(SOCK(s, id), ELEVATING);
	return (0);
}
