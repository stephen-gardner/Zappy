/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 09:57:10 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/24 11:06:22 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include "zappy.h"

static int	parse_int(char *opt, char *arg)
{
	char	*endptr;
	long	n;

	endptr = NULL;
	n = strtol(arg, &endptr, 10);
	if (*endptr)
		FATAL("Invalid %s: %s", opt, arg);
	if (n < 1)
		FATAL("%s must be greater than 0: %s", opt, arg);
	if (n > INT_MAX)
		FATAL("%s too large: %s", opt, arg);
	return ((int)n);
}

static int	parse_short(char *opt, char *arg)
{
	int	n;

	n = parse_int(opt, arg);
	if (n > USHRT_MAX)
		FATAL("%s too large: %s", opt, arg);
	return ((unsigned short)n);
}

void		parse_options(t_serv *s, int ac, char *const av[])
{
	const char	*optstring = "c:n:p:t:x:y:";
	char		f;

	while ((f = getopt(ac, av, optstring)) != -1)
	{
		if (f == 'c')
			s->capacity = parse_int("capacity", optarg);
		else if (f == 'n')
		{
			--optind;
			while (optind < ac && *av[optind] != '-')
				add_team(s, av[optind++]);
		}
		else if (f == 'p')
			s->opt.addr.sin_port = htons(parse_short("port", optarg));
		else if (f == 't')
			s->opt.tick_rate = parse_int("tick rate", optarg);
		else if (f == 'x')
			s->opt.map_width = parse_int("map width", optarg);
		else if (f == 'y')
			s->opt.map_height = parse_int("map height", optarg);
		else
			usage_error(NULL);
	}
}
