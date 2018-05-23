/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 07:45:32 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/23 12:08:58 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"

/*
** Doubles the size of conns and polls
** Zeroes out the memory in the extended memory section
*/

static void	scale_capacity(t_serv *s)
{
	int	half;

	half = s->capacity;
	s->capacity *= 2;
	if (!(s->conns = realloc(s->conns, sizeof(t_conn) * (s->capacity + 1))))
		fatal_error();
	memset(s->conns + half, 0, half);
	if (!(s->polls = realloc(s->polls, sizeof(t_poll) * (s->capacity + 1))))
		fatal_error();
	memset(s->polls + half, 0, half);
}

/*
** Sets the fd and events in the next empty poll data for provided socket
** Scales capacity if needed
** Increases nsockets by 1
** Returns index
*/

int			add_socket(t_serv *s, int sock)
{
	if (s->nsockets == s->capacity)
		scale_capacity(s);
	s->polls[s->nsockets].fd = sock;
	s->polls[s->nsockets].events = (POLLIN | POLLOUT);
	return (s->nsockets++);
}

/*
** Starts listening at local address on specified port
** Adds socket opened to tracked connections
*/

void		init_listener(t_serv *s, char *port)
{
	t_sockin	addr;
	int			fd;
	int			opt;

	opt = 1;
	addr.sin_family = AF_INET;
	addr.sin_port = htons((port) ? atoi(port) : 4242);
	addr.sin_addr.s_addr = INADDR_ANY;
	printf(PNAME " starting...\n");
	fd = socket(PF_INET, SOCK_STREAM, getprotobyname("TCP")->p_proto);
	if (fd < 0
		|| setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int)) < 0
		|| bind(fd, (t_sock *)&s->addr, sizeof(t_sockin)) < 0
		|| listen(fd, SOMAXCONN) < 0)
		fatal_error();
	printf("Listening on tcp://%s:%hu\nUse Ctrl-C to stop\n\n",
		inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	add_client(s, fd);
}

/*
** Closes the socket located at provided index, and removes its data from conns
**  and polls
** Decreases nsockets by 1
*/

void		remove_socket(t_serv *s, int id)
{
	close(s->polls[id].fd);
	// TODO: Remove entity associated with connection ?
	//       Maybe allow another client to take over until it dies ?
	memmove(s->conns + id, s->conns + id + 1,
		sizeof(t_conn) * (s->nsockets - id));
	memmove(s->polls + id, s->polls + id + 1,
		sizeof(t_poll) * (s->nsockets - id));
	--s->nsockets;
}
