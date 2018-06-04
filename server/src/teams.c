/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teams.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 09:34:14 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/04 01:33:06 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zappy.h"

static t_team	*find_team(t_serv *s, char *name)
{
	int	i;

	i = 0;
	while (i < s->nteams)
	{
		if (!strcmp(name, s->teams[i].name))
			return (&s->teams[i]);
		++i;
	}
	return (NULL);
}

void			add_player(t_serv *s, char *name, int id)
{
	t_team	*team;
	t_ent	*ent;

	ent = ENT(s, id);
	if (ent->team || !(team = find_team(s, name)))
	{
		send_message(s, id, "ko\n", 3);
		return (remove_socket(s, id));
	}
	cmd_connect_nbr(s, team, id, 1);
	send_response(s, id);
	if (team->members[0] == team->authorized)
		return (remove_socket(s, id));
	printf("<%s> joined team: %s\n", ent->addr, team->name);
	ent->team = team;
	ent->level = 1;
	ent->inv[FOOD] = 10;
	ent->loc_x = rand() % s->map.width;
	ent->loc_y = rand() % s->map.height;
	++team->members[0];
	++team->members[1];
	--s->conn.capacity;
}

void			add_team(t_serv *s, char *name)
{
	t_team	*team;

	if (!*name)
		return ;
	if (strlen(name) > TEAM_MAX_LEN)
		errx(1, "Team name exceeds max length (%d): %s", TEAM_MAX_LEN, name);
	if (find_team(s, name))
		errx(1, "Duplicate team name: %s", name);
	s->teams = realloc(s->teams, SZ(t_team, s->nteams + 1));
	if (!s->teams)
		err(1, NULL);
	team = &s->teams[s->nteams++];
	memset(team, 0, sizeof(t_team));
	stpcpy(team->name, name);
}
