/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_incantation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 02:29:34 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/14 15:30:19 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <strings.h>
#include "zappy.h"

const t_elevreq	g_elevreq[] = {
	{ 1, { 0, 1, 0, 0, 0, 0, 0, 0 } },
	{ 2, { 0, 1, 1, 1, 0, 0, 0, 0 } },
	{ 2, { 0, 2, 0, 1, 0, 2, 0, 0 } },
	{ 4, { 0, 1, 1, 2, 0, 1, 0, 0 } },
	{ 4, { 0, 1, 2, 1, 3, 0, 0, 0 } },
	{ 6, { 0, 1, 2, 3, 0, 1, 0, 0 } },
	{ 6, { 0, 2, 2, 2, 2, 2, 1, 0 } }
};

static int		is_ready(t_ent *pent, t_ent *cent, int finished)
{
	t_buff	*pbuff;
	t_buff	*cbuff;

	if (cent == pent
		|| cent->level != pent->level
		|| cent->loc_x != pent->loc_x
		|| cent->loc_y != pent->loc_y)
		return (0);
	if (finished)
	{
		pbuff = CMD_NEXT(&pent->cmds);
		cbuff = CMD_NEXT(&cent->cmds);
		if (!cent->cmds.ncmds
			|| cbuff->type != INCANTATION
			|| cbuff->scheduled != pbuff->scheduled)
			return (0);
	}
	else if (cent->cmds.ncmds)
		return (0);
	return (1);
}

static int		count_ready(t_serv *s, t_ent *pent, int finished)
{
	t_ent	*cent;
	int		count;
	int		i;

	i = 1;
	count = 1;
	while (i < s->conn.nsockets)
	{
		cent = ENT(s, i);
		if (is_ready(pent, cent, finished))
			++count;
		++i;
	}
	return (count);
}

void			cmd_incant(t_serv *s, int id)
{
	const t_elevreq	*req;
	t_ent			*pent;
	t_ent			*cent;
	int				count;
	int				i;

	pent = ENT(s, id);
	req = &g_elevreq[pent->level - 1];
	if (count_ready(s, pent, 1) < req->nplayers)
		return ;
	i = 1;
	count = 1;
	while (count < req->nplayers && i < s->conn.nsockets)
	{
		cent = ENT(s, i);
		if (cent != pent && is_ready(pent, cent, 1))
		{
			level_up(cent);
			send_response(s, i);
			++count;
		}
		++i;
	}
	level_up(pent);
}

static void		begin_incant(t_serv *s, t_ent *pent)
{
	const t_elevreq	*req;
	t_ent			*cent;
	t_buff			*cbuff;
	int				count;
	int				i;

	i = 1;
	count = 1;
	req = &g_elevreq[pent->level - 1];
	while (count < req->nplayers && i < s->conn.nsockets)
	{
		cent = ENT(s, i);
		if (is_ready(pent, cent, 0))
		{
			++cent->cmds.ncmds;
			cbuff = CMD_NEXT(&cent->cmds);
			memcpy(cbuff, CMD_NEXT(&pent->cmds), sizeof(t_buff));
			cbuff->pre = 1;
			cbuff->scheduled = s->time + get_cmddef(INCANTATION)->delay;
			send_message(s, i, "elevation in progress\n", 22);
			++count;
		}
		++i;
	}
}

int				precmd_incant(t_serv *s, int id)
{
	const t_elevreq	*req;
	t_ent			*ent;
	t_uint			*loc;
	int				i;

	ent = ENT(s, id);
	if (ent->level >= MAX_LEVEL)
		return (0);
	req = &g_elevreq[ent->level - 1];
	if (count_ready(s, ent, 0) < req->nplayers)
		return (0);
	i = 0;
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	while (i < NRES)
	{
		if (RES_GET(loc, i) < req->nitems[i])
			return (0);
		++i;
	}
	i = -1;
	while (++i < NRES)
		modify_resource(loc, i, -req->nitems[i]);
	begin_incant(s, ent);
	send_message(s, id, "elevation in progress\n", 22);
	return (1);
}
