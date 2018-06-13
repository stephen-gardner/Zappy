/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_inventory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 19:33:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/11 21:03:53 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "zappy.h"

void		cmd_inventory(t_serv *s, int id)
{
	t_ent	*ent;
	t_buff	*buff;
	int		i;

	ent = ENT(s, id);
	buff = CMD_NEXT(&ent->cmds);
	buff->resp_len = sprintf(buff->resp, "{");
	i = 0;
	while (i < g_items_count)
	{
		buff->resp_len += sprintf(buff->resp + buff->resp_len, "%s %u",
			g_items[i], ent->inv[i]);
		if (i + 1 < g_items_count)
			buff->resp_len += sprintf(buff->resp + buff->resp_len, ", ");
		++i;
	}
	buff->resp_len += sprintf(buff->resp + buff->resp_len, "}\n");
}

int			precmd_put(t_serv *s, int id)
{
	t_ent		*ent;
	t_buff		*buff;

	ent = ENT(s, id);
	buff = CMD_NEXT(&ent->cmds);
	return (get_item_id(strchr(buff->recv, ' ') + 1) != -1);
}

void		cmd_put(t_serv *s, int id)
{
	t_ent		*ent;
	t_buff		*buff;
	t_uint		*loc;
	int			item_id;

	ent = ENT(s, id);
	buff = CMD_NEXT(&ent->cmds);
	item_id = get_item_id(strchr(buff->recv, ' ') + 1);
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	if (!ent->inv[item_id] || modify_resource(loc, item_id, 1) < 0)
		return ;
	--ent->inv[item_id];
	buff->resp_len = sprintf(buff->resp, "ok\n");
}

int			precmd_take(t_serv *s, int id)
{
	t_ent		*ent;
	t_buff		*buff;

	ent = ENT(s, id);
	buff = CMD_NEXT(&ent->cmds);
	return (get_item_id(strchr(buff->recv, ' ') + 1) != -1);
}

void		cmd_take(t_serv *s, int id)
{
	t_ent		*ent;
	t_buff		*buff;
	t_uint		*loc;
	int			item_id;

	ent = ENT(s, id);
	buff = CMD_NEXT(&ent->cmds);
	item_id = get_item_id(strchr(buff->recv, ' ') + 1);
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	if (ent->inv[item_id] == USHRT_MAX || modify_resource(loc, item_id, -1) < 0)
		return ;
	++ent->inv[item_id];
	buff->resp_len = sprintf(buff->resp, "ok\n");
}
