/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_inventory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 19:33:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/04 20:03:04 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
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
		buff->resp_len += sprintf(buff->resp + buff->resp_len, "%s %d",
			g_items[i], ent->inv[i]);
		if (i + 1 < g_items_count)
			buff->resp_len += sprintf(buff->resp + buff->resp_len, ", ");
		++i;
	}
	buff->resp_len += sprintf(buff->resp + buff->resp_len, "}\n");
}
