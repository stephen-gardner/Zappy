/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 09:57:10 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/03 01:29:06 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include "zappy.h"

static int			parse_int(char *opt, char *arg)
{
	char	*endptr;
	long	n;

	endptr = NULL;
	n = strtol(arg, &endptr, 10);
	if (*endptr)
		errx(1, "Invalid %s: %s", opt, arg);
	if (n < 1)
		errx(1, "%s must be greater than zero: %s", opt, arg);
	if (n > INT_MAX)
		errx(1, "%s too large: %s", opt, arg);
	return ((int)n);
}

static t_ushrt		parse_short(char *opt, char *arg)
{
	int	n;

	n = parse_int(opt, arg);
	if (n > USHRT_MAX)
		errx(1, "%s too large: %s", opt, arg);
	return ((t_ushrt)n);
}

static t_timespec	parse_tickrate(char *arg)
{
	t_timespec	tickrate;
	int			ticks;

	ticks = parse_int("tick rate", arg);
	if (ticks > 1)
	{
		tickrate.tv_sec = 0;
		tickrate.tv_nsec = 1000000000 / ticks;
	}
	else
	{
		tickrate.tv_sec = 1;
		tickrate.tv_nsec = 0;
	}
	return (tickrate);
}

void				parse_opt(t_serv *s, int ac, char *const av[], char *optstr)
{
	char		f;

	while ((f = getopt(ac, av, optstr)) != -1)
	{
		if (f == 'c')
			s->conn.capacity = parse_int("capacity", optarg);
		else if (f == 'n')
		{
			add_team(s, optarg);
			while (optind < ac && *av[optind] != '-')
				add_team(s, av[optind++]);
		}
		else if (f == 'p')
			s->addr.sin_port = htons(parse_short("port", optarg));
		else if (f == 's')
			srand(parse_int("seed", optarg));
		else if (f == 't')
			s->tickrate = parse_tickrate(optarg);
		else if (f == 'x')
			s->map.width = parse_int("map width", optarg);
		else if (f == 'y')
			s->map.height = parse_int("map height", optarg);
		else
			usage_error(NULL);
	}
}

void				validate_opt(t_serv *s)
{
	if (!s->nteams)
		usage_error("No teams specified");
	if (s->conn.capacity % s->nteams)
		usage_error("Capacity must be a multiple of # teams");
}
