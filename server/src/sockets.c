/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 07:45:32 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/24 22:06:08 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"

static void		scale_user_max(t_conn *c)
{
	int	half;

	half = c->user_max;
	c->user_max *= 2;
	c->ents = realloc(c->ents, SZ(t_ent, c->user_max + 1));
	if (!c->ents)
		err(1, NULL);
	memset(c->ents + half, 0, SZ(t_ent, half + 1));
	c->polls = realloc(c->polls, SZ(t_poll, c->user_max + 1));
	if (!c->polls)
		err(1, NULL);
	memset(c->polls + half, 0, SZ(t_poll, half + 1));
}

static t_ent	*add_socket(t_serv *s, int sock)
{
	if (s->conn.nsockets == s->conn.user_max)
		scale_user_max(&s->conn);
	s->conn.polls[s->conn.nsockets].fd = sock;
	s->conn.polls[s->conn.nsockets].events = POLLIN;
	return (ENT(s, s->conn.nsockets++));
}

void			accept_incoming(t_serv *s)
{
	t_sockin	addr;
	socklen_t	addr_len;
	t_ent		*ent;
	int			sock;
	int			opt;

	opt = 1;
	addr_len = sizeof(addr);
	if ((sock = accept(SOCK(s, 0), (t_sock *)&addr, &addr_len)) == -1
		|| setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) == -1)
		return ;
	ent = add_socket(s, sock);
	ent->feed_time = s->time + (s->ticks * TIMEOUT);
	sprintf(ent->addr, "%s:%hu", inet_ntoa(addr.sin_addr),
		ntohs(addr.sin_port));
	dprintf(sock, WELCOME);
	info(s, "<%s> connected", ent->addr);
}

void			init_listener(t_serv *s)
{
	int			sock;
	int			opt;

	opt = 1;
	s->addr.sin_family = AF_INET;
	s->addr.sin_addr.s_addr = INADDR_ANY;
	sock = socket(PF_INET, SOCK_STREAM, getprotobyname("TCP")->p_proto);
	if (sock == -1
		|| setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1
		|| bind(sock, (t_sock *)&s->addr, sizeof(t_sockin)) == -1
		|| listen(sock, SOMAXCONN) == -1)
		err(1, NULL);
	printf("Listening on tcp://%s:%hu\nUse Ctrl-C to stop\n\n",
		inet_ntoa(s->addr.sin_addr), ntohs(s->addr.sin_port));
	add_socket(s, sock);
}

void			remove_socket(t_serv *s, int id)
{
	struct s_ent	*ent;
	t_team			*team;
	int				sock;

	ent = ENT(s, id);
	sock = SOCK(s, id);
	if ((team = ent->team))
	{
		--team->members[0];
		--team->members[ent->level];
		drop_stones(s, ent);
		dprintf(sock, DEATH);
		info(s, "<%s[%s]> has died", ent->addr, team->name);
	}
	close(sock);
	info(s, "<%s> disconnected", ent->addr);
	memmove(ent, ent + 1, SZ(t_ent, s->conn.nsockets - id));
	memmove(POLL(s, id), POLL(s, id + 1), SZ(t_poll, s->conn.nsockets - id));
	--s->conn.nsockets;
	if (!count_players(s) && !s->conn.capacity && !s->neggs)
		end_game(s, NULL);
}
