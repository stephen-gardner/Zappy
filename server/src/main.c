/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/21 05:50:28 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/09 19:56:30 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zappy.h"

const char	*g_pname;

static void	init_server(t_serv *s)
{
	int	i;
	int	authorized;

	printf("%s starting...\n", g_pname);
	i = 0;
	authorized = s->conn.capacity / s->nteams;
	while (i < s->nteams)
		s->teams[i++].authorized = authorized;
	s->map.size = s->map.height * s->map.width;
	if (!(s->conn.ents = calloc(s->conn.user_max + 1, sizeof(t_ent)))
		|| !(s->conn.polls = calloc(s->conn.user_max + 1, sizeof(t_poll)))
		|| !(s->map.data = calloc(s->map.size, sizeof(t_uint))))
		err(1, NULL);
	init_listener(s);
}

static int	process_queue(t_serv *s, int id)
{
	t_ent	*ent;
	t_poll	*entpoll;
	t_cmd	*cmds;
	t_buff	*buff;

	entpoll = POLL(s, id);
	if ((entpoll->revents & (POLLERR | POLLHUP))
		|| (READABLE(s, id) && read_socket(s, id) < 0))
	{
		remove_socket(s, id);
		return (id);
	}
	ent = ENT(s, id);
	cmds = GET_CMDS(s, id);
	while (WRITABLE(s, id) && cmds->ncmds)
	{
		buff = &cmds->buffs[cmds->start];
		if (buff->type != UNDEFINED && !buff->pre)
			process_precommand(s, id);
		if (buff->scheduled > s->time && ent->team)
			break ;
		process_command(s, id);
		poll(entpoll, 1, 0);
	}
	return (id + 1);
}

static void	server_loop(t_serv *s)
{
	t_timespec	t1;
	t_timespec	t2;
	int			id;

	clock_gettime(CLOCK_MONOTONIC, &t1);
	while (1)
	{
		poll(s->conn.polls, s->conn.nsockets, 0);
		if (READABLE(s, 0))
			accept_incoming(s);
		clock_gettime(CLOCK_MONOTONIC, &t2);
		t2 = time_diff(s->tickrate, time_diff(t2, t1));
		if (t2.tv_sec < 0 || (!t2.tv_sec && !t2.tv_nsec))
		{
			++s->time;
			clock_gettime(CLOCK_MONOTONIC, &t1);
		}
		id = 1;
		while (id < s->conn.nsockets)
			id = process_queue(s, id);
	}
}

int			main(int ac, char *const av[])
{
	static t_serv	s;

	g_pname = av[0];
	memset(&s, 0, sizeof(t_serv));
	s.addr.sin_port = htons(4242);
	s.conn.capacity = 1;
	s.conn.user_max = 10;
	srand(time(NULL));
	s.tickrate.tv_sec = 1;
	s.tickrate.tv_nsec = 0;
	s.map.height = 10;
	s.map.width = 10;
	parse_opt(&s, ac, av, "c:n:p:s:t:x:y:");
	validate_opt(&s);
	init_server(&s);
	server_loop(&s);
	return (0);
}
