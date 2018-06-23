/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_kick.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 22:29:11 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 22:43:32 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

static void	kick_player(t_serv *s, t_ent *cent, int sock, int dir)
{
	static const char	*kickdir[4] = {
		"5317",
		"7531",
		"1753",
		"3175"
	};

	move_dir(s, cent, dir);
	dprintf(sock, "moving %c\n", kickdir[dir / 2][cent->facing / 2]);
}

int			ev_kick(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_ent	*cent;
	int		count;
	int		i;

	(void)id;
	(void)buff;
	i = 1;
	count = 0;
	while (i < s->conn.nsockets)
	{
		cent = ENT(s, i);
		if (cent != ent
			&& cent->loc_x == ent->loc_x
			&& cent->loc_y == ent->loc_y)
		{
			kick_player(s, cent, SOCK(s, i), ent->facing);
			++count;
		}
		++i;
	}
	if (count)
		OK(s);
	return ((count) ? 0 : -1);
}
