/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:08:47 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/23 12:09:49 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_H
# define ZAPPY_H
# include <arpa/inet.h>
# include <poll.h>

typedef struct sockaddr		t_sock;
typedef struct sockaddr_in	t_sockin;
typedef struct pollfd		t_poll;

# define PNAME				"server"

# define CMD_MAX_LEN		256
# define CMD_MAX_REQ		10
# define CMD_TAIL(b)		((b->head + b->ncmds) % CMD_MAX_REQ)
# define BUFF_SIZE			(CMD_MAX_LEN * CMD_MAX_REQ)

# define SOCK(s, id)		s->polls[id].fd
# define WRITEABLE(s, id)	(s->polls[id].revents & POLLOUT)
# define READABLE(s, id)	(s->polls[id].revents & POLLIN)

typedef struct	s_buff
{
	char		data[CMD_MAX_REQ][CMD_MAX_LEN];
	int			size[CMD_MAX_REQ];
	int			head;
	int			ncmds;
}				t_buff;

typedef struct	s_conn
{
	t_buff		rbuff;
	t_buff		wbuff;
}				t_conn;

typedef struct	s_map
{
	int			*data;
	int			height;
	int			width;
}				t_map;

typedef struct	s_serv
{
	t_sockin	addr;
	t_conn		*conns;
	t_poll		*polls;
	int			authorized;
	int			nsockets;
	int			capacity;
}				t_serv;

/*
** main.c
*/

void			fatal_error(void);

/*
** sockets.c
*/

int				add_socket(t_serv *s, int sock);
void			init_listener(t_serv *s, char *port);
void			remove_socket(t_serv *s, int id);

/*
** write.c
*/

int				send_response(t_serv *s, int id, char *msg, int len);
int				write_buffered(t_serv *s, int id);
#endif
