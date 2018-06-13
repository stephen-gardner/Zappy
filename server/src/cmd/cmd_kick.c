/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_kick.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 22:29:11 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/12 23:32:03 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

const char	*g_kickdir[4] = {
	"5713",
	"3571",
	"1357",
	"7135"
};

static void	kick_player(t_serv *s, int id, int dir)
{
	t_ent		*ent;

	ent = ENT(s, id);
	move_dir(s, ent, dir);
	dprintf(SOCK(s, id), "moving %c\n", g_kickdir[dir][ent->facing]);
}

void		cmd_kick(t_serv *s, int id)
{
	t_ent	*pent;
	t_ent	*cent;
	t_buff	*buff;
	int		count;
	int		i;

	pent = ENT(s, id);
	i = 1;
	count = 0;
	while (i < s->conn.nsockets)
	{
		cent = ENT(s, i);
		if (i != id && cent->loc_x == pent->loc_x && cent->loc_y == pent->loc_y)
		{
			kick_player(s, i, pent->facing);
			++count;
		}
		++i;
	}
	if (count)
	{
		buff = CMD_NEXT(&pent->cmds);
		buff->resp_len = sprintf(buff->resp, "ok\n");
	}
}
