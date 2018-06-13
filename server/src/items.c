/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   items.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 19:38:08 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/13 14:42:56 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "zappy.h"

const char	*g_items[] = {
	"food",
	"linemate",
	"deraumere",
	"sibur",
	"mendiane",
	"phiras",
	"thystame"
};

const int	g_items_count = sizeof(g_items) / sizeof(char *);

int			get_item_id(char *name)
{
	int	i;

	i = 0;
	while (i < g_items_count)
	{
		if (!strcmp(name, g_items[i]))
			return (i);
		++i;
	}
	return (-1);
}

int			starve_player(t_serv *s, int id)
{
	t_ent	*ent;

	ent = ENT(s, id);
	if (!ent->inv[FOOD])
	{
		remove_socket(s, id);
		return (1);
	}
	--ent->inv[FOOD];
	ent->feed_time = s->time + HUNGER;
	return (0);
}
