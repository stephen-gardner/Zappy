/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_bct.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 05:59:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 06:39:16 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "zappy.h"

static int	parse_coord(char *arg, int max)
{
	char	*endptr;
	int		n;

	n = strtol(arg, &endptr, 10);
	if (*endptr || n < 0 || n >= max)
		return (-1);
	return ((int)n);
}

void		bct(t_serv *s, int x, int y)
{
	t_ull	*loc;
	int		type;

	build_message(s, "bct %d %d", x, y);
	loc = GET_LOC(s, x, y);
	type = FOOD;
	while (type < EGG)
		build_message(s, " %d", RES_GET(loc, type++));
	build_message(s, "\n");
}

int			ev_bct(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	char	*data;
	int		x;
	int		y;

	(void)id;
	(void)ent;
	if (!(data = strdup(buff->data)))
		err(1, NULL);
	strsep(&data, " ");
	if ((x = parse_coord(strsep(&data, " "), s->map.width)) == -1
		|| (y = parse_coord(strsep(&data, " "), s->map.height)) == -1)
		return (-1);
	free(data);
	bct(s, x, y);
	return (0);
}
