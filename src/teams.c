/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teams.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 09:34:14 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/24 09:56:18 by sgardner         ###   ########.fr       */
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
	t_opt	*opt;
	t_team	*team;
	int		i;

	if (!*name)
		return ;
	if (strlen(name) > TEAM_MAX_LEN)
		FATAL("Team name \"%s\" exceeds max length (%d)", name, TEAM_MAX_LEN);
	i = 0;
	opt = &s->opt;
	while (i < opt->nteams)
	{
		if (!strcmp(name, opt->teams[i++].name))
			FATAL("Duplicate team name \"%s\"", name);
	}
	opt->teams = realloc(opt->teams, sizeof(t_team) * (opt->nteams + 1));
	if (!opt->teams)
		FATAL(NULL);
	team = &opt->teams[opt->nteams++];
	stpcpy(team->name, name);
	team->authorized = 1;
	team->nclients = 0;
}
