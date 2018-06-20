/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 19:38:08 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/19 21:46:33 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"

void	end_game(t_serv *s, t_team *team)
{
	int	i;

	if (team)
		info(s, "[%s] wins the game! :)", team->name);
	else
		info(s, "All players have died. Game over. :(");
	i = 0;
	while (i < s->conn.nsockets)
		close(SOCK(s, i++));
	exit(1);
}

void	kill_hatchling(t_serv *s, t_egg *egg)
{
	info(s, "[%s] hatchling died of hunger", egg->team->name);
	--s->conn.capacity;
	--egg->team->authorized;
	remove_egg(s, egg);
}

void	level_up(t_serv *s, t_ent *ent, t_buff *buff)
{
	--ent->team->members[ent->level];
	++ent->team->members[++ent->level];
	if (ent->team->members[8] >= 6)
		end_game(s, ent->team);
	buff->resp_len = sprintf(buff->resp, CURR_LEVEL, ent->level);
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
