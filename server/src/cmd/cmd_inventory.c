/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_inventory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 19:33:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 04:05:56 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "zappy.h"

int			cmd_inventory(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	int		i;

	(void)s;
	(void)id;
	build_message(buff, "{");
	i = 0;
	while (i < g_items_count)
	{
		build_message(buff, "%s %u", g_items[i], ent->inv[i]);
		if (i + 1 < g_items_count)
			build_message(buff, ", ");
		++i;
	}
	build_message(buff, "}\n");
	return (0);
}

int			precmd_put(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)s;
	(void)id;
	(void)ent;
	return (get_item_id(strchr(buff->recv, ' ') + 1));
}

int			cmd_put(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_ull		*loc;
	int			item_id;

	(void)s;
	(void)id;
	item_id = get_item_id(strchr(buff->recv, ' ') + 1);
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	if (!ent->inv[item_id] || modify_resource(loc, item_id, 1) < 0)
		return (-1);
	--ent->inv[item_id];
	OK(buff);
	return (0);
}

int			precmd_take(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)s;
	(void)id;
	(void)ent;
	return (get_item_id(strchr(buff->recv, ' ') + 1));
}

int			cmd_take(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_ull		*loc;
	int			item_id;

	(void)id;
	item_id = get_item_id(strchr(buff->recv, ' ') + 1);
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	if (ent->inv[item_id] == USHRT_MAX || modify_resource(loc, item_id, -1) < 0)
		return (-1);
	++ent->inv[item_id];
	OK(buff);
	return (0);
}
