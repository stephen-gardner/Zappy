/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/21 05:50:28 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/02 18:20:18 by sgardner         ###   ########.fr       */
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

static void			poll_conns(t_serv *s)
{
	int			id;

	if (READABLE(s, 0))
		accept_incoming(s);
	id = 0;
	while (++id < s->conn.nsockets)
	{
		if ((POLL(s, id)->revents & (POLLERR | POLLHUP))
			|| (READABLE(s, id) && read_socket(s, id) < 0))
		{
			remove_socket(s, id);
			continue ;
		}
		if (WRITABLE(s, id) && GET_CMDS(s, id)->ncmds)
		{
			if (GET_CMDS(s, id)->buffs[0].delay)
				--GET_CMDS(s, id)->buffs[0].delay;
			else
				process_command(s, id);
		}
	}
}

static void			server_loop(t_serv *s)
{
	t_timespec	t1;
	t_timespec	t2;

	while (1)
	{
		clock_gettime(CLOCK_MONOTONIC, &t1);
		if (poll(s->conn.polls, s->conn.nsockets, 0) > 0)
			poll_conns(s);
		clock_gettime(CLOCK_MONOTONIC, &t2);
		t2 = time_diff(&t2, &t1);
		if ((t2 = time_diff(&s->tickrate, &t2)).tv_sec < 0)
			fprintf(stderr, "Server can't keep up!\n");
		else
			nanosleep(&t2, NULL);
	}
}

static void			init_server(t_serv *s)
{
	int	i;
	int	authorized;

	printf("%s starting...\n", g_pname);
	i = 0;
	authorized = s->conn.capacity / s->nteams;
	while (i < s->nteams)
		s->teams[i++].authorized = authorized;
	s->map.size = s->map.height * s->map.width;
	if (!(s->conn.ents = calloc(s->conn.capacity + 1, sizeof(t_ent)))
		|| !(s->conn.polls = calloc(s->conn.capacity + 1, sizeof(t_poll)))
		|| !(s->map.data = calloc(s->map.size, sizeof(t_uint))))
		FATAL(NULL);
	init_listener(s);
	server_loop(s);
}

int					main(int ac, char *const av[])
{
	static t_serv	s;

	g_pname = av[0];
	memset(&s, 0, sizeof(t_serv));
	s.addr.sin_port = htons(4242);
	s.conn.capacity = 1;
	srand(time(NULL));
	s.tickrate.tv_sec = 1;
	s.tickrate.tv_nsec = 0;
	s.map.height = 10;
	s.map.width = 10;
	parse_opt(&s, ac, av, "c:n:p:s:t:x:y:");
	validate_opt(&s);
	init_server(&s);
	return (0);
}
