/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 07:45:32 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/03 01:35:47 by sgardner         ###   ########.fr       */
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
	int			sock;

	addr_len = sizeof(addr);
	while (READABLE(s, 0))
	{
		if ((sock = accept(SOCK(s, 0), (t_sock *)&addr, &addr_len)) != -1)
		{
			if (s->conn.capacity)
			{
				id = add_socket(s, sock);
				sprintf(ENT(s, id)->addr, "%s:%hu", inet_ntoa(addr.sin_addr),
					ntohs(addr.sin_port));
				send_message(s, id, "WELCOME\n", 8);
				printf("* %s connected\n", ENT(s, id)->addr);
			}
			else
				close(sock);
		}
		poll(s->conn.polls, 1, 0);
	}
}

/*
** Doubles the size of ents and polls
** Zeroes out the memory in the extended memory section
*/

static void	scale_user_max(t_conn *c)
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

/*
** Sets the fd and events in the next empty poll data for provided socket
** Scales capacity if needed
** Increases nsockets by 1
** Returns index
*/

int			add_socket(t_serv *s, int sock)
{
	if (s->conn.nsockets == s->conn.user_max)
		scale_user_max(&s->conn);
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
		err(1, NULL);
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
	struct s_ent	*ent;
	t_team			*team;

	ent = ENT(s, id);
	close(SOCK(s, id));
	if ((team = ent->team))
	{
		--team->members[0];
		--team->members[ent->level];
	}
	printf("* %s disconnected\n", ent->addr);
	memmove(ent, ent + 1, SZ(t_ent, s->conn.nsockets - id));
	memmove(POLL(s, id), POLL(s, id + 1), SZ(t_poll, s->conn.nsockets - id));
	--s->conn.nsockets;
}
