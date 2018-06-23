/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 07:45:32 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 20:44:07 by sgardner         ###   ########.fr       */
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

	addr_len = sizeof(addr);
	if ((sock = accept(SOCK(s, 0), (t_sock *)&addr, &addr_len)) != -1)
	{
		if (s->conn.capacity)
		{
			ent = add_socket(s, sock);
			sprintf(ent->addr, "%s:%hu", inet_ntoa(addr.sin_addr),
				ntohs(addr.sin_port));
			dprintf(sock, WELCOME);
			info(s, "<%s> connected", ent->addr);
		}
		else
			close(sock);
	}
}

void			init_listener(t_serv *s)
{
	int			fd;
	int			opt;

	opt = 1;
	s->addr.sin_family = AF_INET;
	s->addr.sin_addr.s_addr = INADDR_ANY;
	fd = socket(PF_INET, SOCK_STREAM, getprotobyname("TCP")->p_proto);
	if (fd < 0
		|| setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0
		|| bind(fd, (t_sock *)&s->addr, sizeof(t_sockin)) < 0
		|| listen(fd, SOMAXCONN) < 0)
		err(1, NULL);
	printf("Listening on tcp://%s:%hu\nUse Ctrl-C to stop\n\n",
		inet_ntoa(s->addr.sin_addr), ntohs(s->addr.sin_port));
	add_socket(s, fd);
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
		dprintf(sock, DEATH);
		info(s, "<%s[%s]> has died", ent->addr, team->name);
	}
	close(sock);
	info(s, "<%s> disconnected", ent->addr);
	drop_stones(s, ent);
	memmove(ent, ent + 1, SZ(t_ent, s->conn.nsockets - id));
	memmove(POLL(s, id), POLL(s, id + 1), SZ(t_poll, s->conn.nsockets - id));
	--s->conn.nsockets;
	if (s->conn.nsockets < 2 && !s->conn.capacity && !s->neggs)
		end_game(s, NULL);
}
