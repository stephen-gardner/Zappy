/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teams.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 09:34:14 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 20:46:07 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zappy.h"

static void	set_location(t_serv *s, t_ent *ent)
{
	t_egg	*egg;

	if ((egg = find_egg(s, ent->team)))
	{
		memcpy(ent->uuid, egg->uuid, sizeof(uuid_t));
		ent->inv[FOOD] = egg->food;
		ent->feed_time = egg->scheduled;
		ent->loc_x = egg->loc_x;
		ent->loc_y = egg->loc_y;
		remove_egg(s, egg);
	}
	else
	{
		uuid_generate(ent->uuid);
		ent->inv[FOOD] = 9;
		ent->feed_time = s->time + HUNGER;
		ent->loc_x = rand() % s->map.width;
		ent->loc_y = rand() % s->map.height;
	}
	if ((ent->facing = rand() % NDIR) % 2)
		CHDIR(ent->facing, 1);
}

void		add_player(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_team	*team;

	if (ent->team || !(team = find_team(s, buff->data)))
	{
		dprintf(SOCK(s, id), "ko\n");
		return (remove_socket(s, id));
	}
	cmd_connect(s, team, 1);
	send_response(s, id);
	if (!team->authorized)
		return (remove_socket(s, id));
	ent->team = team;
	ent->level = 1;
	set_location(s, ent);
	++team->members[0];
	++team->members[1];
	--team->authorized;
	--s->conn.capacity;
}

void		add_team(t_serv *s, char *name)
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

t_team		*find_team(t_serv *s, char *name)
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
