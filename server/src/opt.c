/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 09:57:10 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 07:51:21 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "zappy.h"

void			init_server(t_serv *s)
{
	int	i;
	int	authorized;

	printf("%s starting...\n", g_pname);
	i = 0;
	authorized = s->conn.capacity / s->nteams;
	while (i < s->nteams)
		s->teams[i++].authorized = authorized;
	if (!(s->conn.ents = calloc(s->conn.user_max + 1, sizeof(t_ent)))
		|| !(s->conn.polls = calloc(s->conn.user_max + 1, sizeof(t_poll)))
		|| !(s->map.data = calloc(s->map.size, sizeof(t_ull))))
		err(1, NULL);
	printf("Generating resources...\n");
	populate_map(s);
	init_listener(s);
}

static int		parse_int(char *opt, char *arg)
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

static t_ushrt	parse_short(char *opt, char *arg)
{
	int	n;

	n = parse_int(opt, arg);
	if (n > USHRT_MAX)
		errx(1, "%s too large: %s", opt, arg);
	return ((t_ushrt)n);
}

void			parse_opt(t_serv *s, int ac, char *const av[], char *optstr)
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
			set_tickrate(s, parse_int("tick rate", optarg));
		else if (f == 'x')
			s->map.width = parse_int("map width", optarg);
		else if (f == 'y')
			s->map.height = parse_int("map height", optarg);
		else
			usage_error(NULL);
	}
}

void			validate_opt(t_serv *s)
{
	s->map.size = s->map.height * s->map.width;
	if (!s->nteams)
		usage_error("No teams specified");
	if (s->map.height < 8 || s->map.width < 8)
		usage_error("Minimum map dimensions are 8x8");
	if ((size_t)s->conn.capacity > s->map.size / 4)
		errx(1, "Density too high. Lower capacity or increase map size.");
	if (s->conn.capacity % s->nteams)
		errx(1, "Capacity must be a multiple of # teams");
}
