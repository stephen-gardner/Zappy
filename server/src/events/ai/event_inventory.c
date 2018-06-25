/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_inventory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 19:33:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/24 23:00:02 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "zappy.h"

int			ev_inventory(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	int		i;

	(void)s;
	(void)id;
	(void)buff;
	build_message(s, "{");
	i = 0;
	while (i < g_items_count)
	{
		build_message(s, "%s %u", g_items[i], ent->inv[i]);
		if (i + 1 < g_items_count)
			build_message(s, ", ");
		++i;
	}
	build_message(s, "}\n");
	return (0);
}

int			preev_put(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)s;
	(void)id;
	(void)ent;
	return (get_item_id(strchr(buff->data, ' ') + 1));
}

int			ev_put(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_ull		*loc;
	int			item_id;

	(void)s;
	(void)id;
	item_id = get_item_id(strchr(buff->data, ' ') + 1);
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	if (!ent->inv[item_id] || modify_resource(loc, item_id, 1) == -1)
		return (-1);
	--ent->inv[item_id];
	OK(s);
	return (0);
}

int			preev_take(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)s;
	(void)id;
	(void)ent;
	return (get_item_id(strchr(buff->data, ' ') + 1));
}

int			ev_take(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_ull		*loc;
	int			item_id;

	(void)id;
	item_id = get_item_id(strchr(buff->data, ' ') + 1);
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	if (ent->inv[item_id] == USHRT_MAX
		|| modify_resource(loc, item_id, -1) == -1)
		return (-1);
	++ent->inv[item_id];
	OK(s);
	return (0);
}
