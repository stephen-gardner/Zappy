/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/21 05:50:28 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/24 16:19:38 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zappy.h"

const char	*g_pname;

static void	init_server(t_serv *s)
{
	t_opt	*opt;

	opt = &s->opt;
	printf("%s starting...\n", g_pname);
	if (!(s->conns = calloc(opt->capacity, sizeof(t_conn)))
		|| !(s->polls = calloc(opt->capacity, sizeof(t_poll)))
		|| !(s->map = calloc(s->map_size, sizeof(t_uint))))
		FATAL(NULL);
	init_listener(s);
}

int			main(int ac, char *const av[])
{
	static t_serv	s;

	g_pname = av[0];
	memset(&s, 0, sizeof(t_serv));
	s.opt.addr.sin_port = htons(4242);
	s.opt.capacity = 1;
	s.opt.tick_rate = 1;
	s.opt.map_height = 10;
	s.opt.map_width = 10;
	parse_options(&s, ac, av);
	init_server(&s);
	return (0);
}
