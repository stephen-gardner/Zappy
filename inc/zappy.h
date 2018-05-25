/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:08:47 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/25 12:10:21 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_H
# define ZAPPY_H
# include <arpa/inet.h>
# include <poll.h>
# include <time.h>

typedef struct sockaddr		t_sock;
typedef struct sockaddr_in	t_sockin;
typedef struct pollfd		t_poll;
typedef struct timespec		t_timespec;
typedef unsigned int		t_uint;
typedef unsigned short		t_ushrt;

# define FATAL				fatal_error

# define TEAM_MAX_LEN		27
# define CMD_MAX_LEN		256
# define CMD_MAX_REQ		10
# define CMD_TAIL(b)		((b->head + b->ncmds) % CMD_MAX_REQ)
# define BUFF_SIZE			(CMD_MAX_LEN * CMD_MAX_REQ)

# define SOCK(s, id)		s->polls[id].fd
# define WRITEABLE(s, id)	(s->polls[id].revents & POLLOUT)
# define READABLE(s, id)	(s->polls[id].revents & POLLIN)

/*
** Masks for resource quantity
** 5 bits per resource for max of 31
**	TH         PH       ME         SI      DE          LI
** [THYSTAME] [PHIRAS] [MENDIANE] [SIBUR] [DERAUMERE] [LINEMATE]
*/

# define NMAX				31
# define NLI(i)				(i & 0x1F)
# define NDE(i)				(i & 0x3E0)
# define NSI(i)				(i & 0x7C00)
# define NME(i)				(i & 0xF8000)
# define NPH(i)				(i & 0x1F00000)
# define NTH(i)				(i & 0x3E000000)

/*
** Macros to set resource quantity
*/

# define SLI(i, n)			(i |= n)
# define SDE(i, n)			(i |= (n << 5))
# define SSI(i, n)			(i |= (n << 10))
# define SME(i, n)			(i |= (n << 15))
# define SPH(i, n)			(i |= (n << 20))
# define STH(i, n)			(i |= (n << 25))

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

typedef struct	s_team
{
	char		name[TEAM_MAX_LEN + 1];
	int			authorized;
	int			nclients;
}				t_team;

typedef struct	s_opt
{
	t_sockin	addr;
	t_timespec	tickrate;
	t_team		*teams;
	int			capacity;
	int			map_height;
	int			map_width;
}				t_opt;

typedef struct	s_serv
{
	t_opt		opt;
	t_conn		*conns;
	t_poll		*polls;
	t_uint		*map;
	long		map_size;
	int			nsockets;
	int			nteams;
}				t_serv;

/*
** error.c
*/

void			fatal_error(char *fmt, ...);
void			usage_error(char *msg);

/*
** opt.c
*/

void			parse_options(t_serv *s, int ac, char *const av[]);

/*
** sockets.c
*/

int				add_socket(t_serv *s, int sock);
void			init_listener(t_serv *s);
void			remove_socket(t_serv *s, int id);

/*
** teams.c
*/

void			add_team(t_serv *s, char *name);

/*
** write.c
*/

int				send_response(t_serv *s, int id, char *msg, int len);
int				write_buffered(t_serv *s, int id);

extern const char	*g_pname;
#endif
