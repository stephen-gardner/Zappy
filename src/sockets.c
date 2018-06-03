/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 07:45:32 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/02 18:23:40 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"

void		accept_incoming(t_serv *s)
{
	t_sockin	addr;
	socklen_t	addr_len;
	int			id;

	while (READABLE(s, 0))
	{
		id = add_socket(s, accept(SOCK(s, 0), (t_sock *)&addr, &addr_len));
		if (id > 0)
		{
			send_message(s, id, "WELCOME\n", 8);
			printf("%s:%hu connected\n", inet_ntoa(addr.sin_addr),
				ntohs(addr.sin_port));
		}
		poll(s->conn.polls, 1, 0);
	}
}

/*
** Doubles the size of ents and polls
** Zeroes out the memory in the extended memory section
*/

static void	scale_capacity(t_conn *c)
{
	int	half;

	half = c->capacity;
	c->capacity *= 2;
	c->ents = realloc(c->ents, SZ(t_ent, c->capacity + 1));
	if (!c->ents)
		FATAL(NULL);
	memset(c->ents + half, 0, SZ(t_ent, half + 1));
	c->polls = realloc(c->polls, SZ(t_poll, c->capacity + 1));
	if (!c->polls)
		FATAL(NULL);
	memset(c->polls + half, 0, SZ(t_poll, half + 1));
}

/*
** Sets the fd and events in the next empty poll data for provided socket
** Scales capacity if needed
** Increases nsockets by 1
** Returns index
*/

int			add_socket(t_serv *s, int sock)
{
	if (sock < 0)
		return (-1);
	if (s->conn.nsockets == s->conn.capacity)
		scale_capacity(&s->conn);
	s->conn.polls[s->conn.nsockets].fd = sock;
	s->conn.polls[s->conn.nsockets].events = (POLLIN | POLLOUT);
	return (s->conn.nsockets++);
}

/*
** Starts listening on specified port
** Adds socket opened to tracked connections
*/

void		init_listener(t_serv *s)
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
		FATAL(NULL);
	printf("Listening on tcp://%s:%hu\nUse Ctrl-C to stop\n\n",
		inet_ntoa(s->addr.sin_addr), ntohs(s->addr.sin_port));
	add_socket(s, fd);
}

/*
** Closes the socket located at provided index, and removes its data from ents
**  and polls
** Decreases nsockets by 1
*/

void		remove_socket(t_serv *s, int id)
{
	t_conn	*c;
	t_team	*team;

	c = &s->conn;
	close(SOCK(s, id));
	if ((team = c->ents[id].team))
	{
		--team->members[0];
		--team->members[c->ents[id].level];
	}
	memmove(&c->ents[id], &c->ents[id + 1], SZ(t_ent, c->nsockets - id));
	memmove(&c->polls[id], &c->polls[id + 1], SZ(t_poll, c->nsockets - id));
	--c->nsockets;
}
