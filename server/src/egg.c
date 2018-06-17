/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   egg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 23:44:36 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/14 19:25:23 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "zappy.h"

void		add_egg(t_serv *s, t_team *team, int x, int y)
{
	t_egg	*egg;

	if (!(s->eggs = realloc(s->eggs, SZ(t_egg, s->neggs + 2))))
		err(1, NULL);
	memset(s->eggs + s->neggs, 0, SZ(t_egg, 2));
	egg = &s->eggs[s->neggs++];
	egg->team = team;
	egg->loc_x = x;
	egg->loc_y = y;
	egg->scheduled = s->time + HATCH_TIME;
}

static void	hatch_egg(t_serv *s, t_egg *egg)
{
	t_uint	*loc;

	egg->hatched = 1;
	egg->food = 9;
	egg->scheduled = s->time + HUNGER;
	loc = GET_LOC(s, egg->loc_x, egg->loc_y);
	modify_resource(loc, EGG, -1);
	++s->conn.capacity;
	++egg->team->authorized;
	info(s, "[%s] egg hatched", egg->team->name);
}

t_egg		*find_egg(t_serv *s, t_team *team)
{
	t_egg	*egg;
	int		i;

	i = 0;
	while (i < s->neggs)
	{
		egg = &s->eggs[i++];
		if (egg->hatched && egg->team == team)
			return (egg);
	}
	return (NULL);
}

void		incubate(t_serv *s)
{
	t_egg	*egg;
	int		i;

	i = 0;
	while (i < s->neggs)
	{
		egg = &s->eggs[i];
		if (s->time == egg->scheduled)
		{
			if (egg->hatched)
			{
				if (!egg->food)
				{
					kill_hatchling(s, egg);
					continue ;
				}
				--egg->food;
				egg->scheduled = s->time + HUNGER;
			}
			else
				hatch_egg(s, egg);
		}
		++i;
	}
}

void		remove_egg(t_serv *s, t_egg *egg)
{
	int	i;

	i = (egg - s->eggs) / sizeof(t_egg);
	memmove(egg, egg + 1, SZ(t_egg, s->neggs - i));
	--s->neggs;
}
