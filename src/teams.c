/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teams.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 09:34:14 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/25 20:30:59 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "zappy.h"

/*
** Adds team name to list of tracked teams
** Exits if team name is too long or duplicate
** Returns if name is empty
*/

void	add_team(t_serv *s, char *name)
{
	t_team	*team;
	int		i;

	if (!*name)
		return ;
	if (strlen(name) > TEAM_MAX_LEN)
		FATAL("Team name \"%s\" exceeds max length (%d)", name, TEAM_MAX_LEN);
	i = 0;
	while (i < s->nteams)
	{
		if (!strcmp(name, s->teams[i++].name))
			FATAL("Duplicate team name \"%s\"", name);
	}
	s->teams = realloc(s->teams, sizeof(t_team) * (s->nteams + 1));
	if (!s->teams)
		FATAL(NULL);
	team = &s->teams[s->nteams++];
	memset(team, 0, sizeof(t_team));
	stpcpy(team->name, name);
}
