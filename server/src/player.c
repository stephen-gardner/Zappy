/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 19:38:08 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 07:32:34 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

int		count_players(t_serv *s)
{
	int	count;
	int	i;

	i = 1;
	count = 0;
	while (i < s->conn.nsockets)
	{
		if (ENT(s, i)->type == ENT_PLAYER)
			++count;
		++i;
	}
	return (count);
}

t_ent	*find_player(t_serv *s, intmax_t uid)
{
	t_ent	*ent;
	int		i;

	i = 1;
	while (i < s->conn.nsockets)
	{
		ent = ENT(s, i++);
		if (ent->type == ENT_PLAYER && ent->uid == uid)
			return (ent);
	}
	return (NULL);
}

void	kill_hatchling(t_serv *s, t_egg *egg)
{
	info(s, "[%s] hatchling died of hunger", egg->team->name);
	--s->conn.capacity;
	--egg->team->authorized;
	remove_egg(s, egg);
}

void	level_up(t_serv *s, t_ent *ent)
{
	--ent->team->members[ent->level];
	++ent->team->members[++ent->level];
	if (ent->team->members[MAX_LEVEL] >= REQ_MAX_PLAYERS)
		end_game(s, ent->team);
	build_message(s, CURR_LEVEL, ent->level);
}

int		starve_player(t_serv *s, t_ent *ent)
{
	if (!ent->inv[FOOD])
		return (1);
	--ent->inv[FOOD];
	ent->feed_time = s->time + HUNGER;
	replenish(s, FOOD, 1);
	return (0);
}
