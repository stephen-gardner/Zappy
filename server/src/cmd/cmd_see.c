/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_see.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/10 01:43:44 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/19 07:42:10 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "zappy.h"

static void	move_rel(t_serv *s, t_move *m, int dir)
{
	if (dir == EAST)
	{
		MOVE(m->loc_x, s->map.width, -m->rel_y);
		MOVE(m->loc_y, s->map.height, m->rel_x);
	}
	else if (dir == WEST)
	{
		MOVE(m->loc_x, s->map.width, m->rel_y);
		MOVE(m->loc_y, s->map.height, -m->rel_x);
	}
	else if (dir == SOUTH)
	{
		MOVE(m->loc_x, s->map.width, -m->rel_x);
		MOVE(m->loc_y, s->map.height, -m->rel_y);
	}
	else
	{
		MOVE(m->loc_x, s->map.width, m->rel_x);
		MOVE(m->loc_y, s->map.height, m->rel_y);
	}
}

static void	print_items(t_buff *buff, t_uint *loc)
{
	int	type;
	int	n;

	type = 0;
	while (type < EGG)
	{
		n = RES_GET(loc, type);
		while (n--)
		{
			buff->resp_len += sprintf(buff->resp + buff->resp_len, "%s ",
				g_items[type]);
		}
		++type;
	}
	if (buff->resp_len < 2
		|| !strncmp(&buff->resp[buff->resp_len - 2], ", ", 2))
		buff->resp_len += sprintf(buff->resp + buff->resp_len, ", ");
	else
		buff->resp_len += sprintf(buff->resp + buff->resp_len - 1, ", ") - 1;
}

static void	print_players(t_serv *s, t_ent *ent, t_buff *buff, t_move *m)
{
	t_ent	*cent;
	int		i;

	i = 1;
	while (i < s->conn.nsockets)
	{
		cent = ENT(s, i++);
		if (cent != ent
			&& cent->loc_x == m->loc_x
			&& cent->loc_y == m->loc_y)
			buff->resp_len += sprintf(buff->resp + buff->resp_len, "player ");
	}
}

static void	see(t_serv *s, t_ent *ent, t_buff *buff, t_move *m)
{
	int		row;
	int		col;

	row = 0;
	while (row <= ent->level)
	{
		col = -row;
		while (col <= row)
		{
			print_players(s, ent, buff, m);
			print_items(buff, GET_LOC(s, m->loc_x, m->loc_y));
			m->rel_y = 0;
			m->rel_x = 1;
			move_rel(s, m, ent->facing);
			++col;
		}
		m->rel_y = -1;
		m->rel_x = -(++row * 2);
		move_rel(s, m, ent->facing);
	}
}

void		cmd_see(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_move	m;

	(void)id;
	m.loc_x = ent->loc_x;
	m.loc_y = ent->loc_y;
	m.rel_x = 0;
	m.rel_y = 0;
	buff->resp_len = sprintf(buff->resp, "{");
	see(s, ent, buff, &m);
	stpcpy(buff->resp + buff->resp_len - 2, "}\n");
}
