/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 19:38:08 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/18 06:42:20 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	kill_hatchling(t_serv *s, t_egg *egg)
{
	info(s, "[%s] hatchling died of hunger", egg->team->name);
	--s->conn.capacity;
	--egg->team->authorized;
	remove_egg(s, egg);
}

void	level_up(t_serv *s, t_ent *ent)
{
	t_buff	*buff;

	--ent->team->members[ent->level];
	++ent->team->members[++ent->level];
	if (ent->team->members[8] >= 6)
	{
		s->go = 0;
		info(s, "[%s] wins the game!", ent->team->name);
	}
	buff = CMD_NEXT(&ent->cmds);
	buff->resp_len = sprintf(buff->resp, "current level : %d\n", ent->level);
}

int		starve_player(t_serv *s, int id)
{
	t_ent	*ent;

	ent = ENT(s, id);
	if (!ent->inv[FOOD])
		return (1);
	--ent->inv[FOOD];
	ent->feed_time = s->time + HUNGER;
	replenish(s, FOOD, 1);
	return (0);
}
