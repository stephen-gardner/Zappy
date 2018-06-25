/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/21 05:50:28 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 07:53:37 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"

const char	*g_pname;

void		end_game(t_serv *s, t_team *team)
{
	int	i;

	if (team)
		info(s, "[%s] wins the game! :)", team->name);
	else
		info(s, "All players have died. Game over. :(");
	i = 0;
	while (i < s->conn.nsockets)
		close(SOCK(s, i++));
	exit(1);
}

static int	process_entity(t_serv *s, int id, t_ent *ent)
{
	t_buff	*buff;

	if (((ent->type == ENT_PLAYER || ent->type == ENT_UNDEFINED)
			&& ent->feed_time == s->time
			&& starve_player(s, ent))
		|| (POLL(s, id)->revents & (POLLERR | POLLHUP))
		|| (READABLE(s, id) && read_socket(s, id, ent) == -1))
	{
		remove_socket(s, id);
		return (1);
	}
	while (ent->evs.nevs)
	{
		buff = EV_NEXT(&ent->evs);
		if (!buff->pre)
			process_precommand(s, id, ent, buff);
		if ((ent->team || ent->type == ENT_GRAPHIC)
			&& buff->scheduled != s->time
			&& buff->type != UNDEFINED)
			break ;
		process_command(s, id, ent, buff);
	}
	return (0);
}

static void	run_events(t_serv *s)
{
	int		id;

	if (s->neggs)
		incubate(s);
	if (READABLE(s, 0))
		accept_incoming(s);
	id = 1;
	while (id < s->conn.nsockets)
	{
		if (process_entity(s, id, ENT(s, id)))
			continue ;
		++id;
	}
}

#define MSEC(ts)	(ts.tv_sec * 1000) + (ts.tv_nsec / 1000000)

static void	server_loop(t_serv *s)
{
	t_timespec	t1;
	t_timespec	t2;
	int			timeout;

	clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
	timeout = MSEC(s->tickrate);
	while (poll(s->conn.polls, s->conn.nsockets, timeout) != -1)
	{
		run_events(s);
		clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
		t2 = time_diff(s->tickrate, time_diff(t2, t1));
		if (t2.tv_sec < 0 || (!t2.tv_sec && !t2.tv_nsec))
		{
			++s->time;
			clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
			timeout = MSEC(s->tickrate);
		}
		else
			timeout = MSEC(t2);
	}
}

int			main(int ac, char *const av[])
{
	static t_serv	s;

	g_pname = av[0];
	srand(time(NULL));
	s.addr.sin_port = htons(4242);
	s.conn.capacity = 1;
	s.conn.user_max = 10;
	s.map.height = 10;
	s.map.width = 10;
	s.tickrate.tv_sec = 1;
	s.tickrate.tv_nsec = 0;
	s.ticks = 1;
	parse_opt(&s, ac, av, "c:n:p:s:t:x:y:");
	validate_opt(&s);
	init_server(&s);
	server_loop(&s);
	return (0);
}
