/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/21 05:50:28 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/25 12:27:53 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "zappy.h"

const char	*g_pname;

static t_timespec	time_diff(struct timespec *t1, struct timespec *t2)
{
	struct timespec	res;

	res.tv_sec = t1->tv_sec - t2->tv_sec;
	if ((res.tv_nsec = t1->tv_nsec - t2->tv_nsec) < 0)
	{
		--res.tv_sec;
		res.tv_nsec += 1000000000;
	}
	return (res);
}

static void			loop(t_serv *s)
{
	t_timespec	t1;
	t_timespec	t2;
	int			i;

	i = 0;
	while (1)
	{
		clock_gettime(CLOCK_MONOTONIC, &t1);
		/*
		**
		** STUFF HAPPENS HERE
		**
		*/
		printf("i = %d\n", i++);
		clock_gettime(CLOCK_MONOTONIC, &t2);
		t2 = time_diff(&t2, &t1);
		t2 = time_diff(&s->opt.tickrate, &t2);
		if (t2.tv_sec < 0)
			fprintf(stderr, "Server can't keep up!\n");
		else
			nanosleep(&t2, NULL);
	}
}

static void			init_server(t_serv *s)
{
	t_opt	*opt;

	opt = &s->opt;
	printf("%s starting...\n", g_pname);
	if (!(s->conns = calloc(opt->capacity + 1, sizeof(t_conn)))
		|| !(s->polls = calloc(opt->capacity + 1, sizeof(t_poll)))
		|| !(s->map = calloc(s->map_size, sizeof(t_uint))))
		FATAL(NULL);
	init_listener(s);
	loop(s);
}

int					main(int ac, char *const av[])
{
	static t_serv	s;

	g_pname = av[0];
	memset(&s, 0, sizeof(t_serv));
	s.opt.addr.sin_port = htons(4242);
	s.opt.capacity = 1;
	s.opt.tickrate.tv_sec = 1;
	s.opt.tickrate.tv_nsec = 0;
	s.opt.map_height = 10;
	s.opt.map_width = 10;
	parse_options(&s, ac, av);
	init_server(&s);
	return (0);
}
