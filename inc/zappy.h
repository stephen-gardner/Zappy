/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:08:47 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/28 17:25:01 by sgardner         ###   ########.fr       */
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
# define SZ(x, n)			(sizeof(x) * (n))

# define CMD_MAX_LEN		256
# define CMD_MAX_REQ		10
# define CMD_TAIL(b)		((b->head + b->ncmds) % CMD_MAX_REQ)
# define BUFF_SIZE			(CMD_MAX_LEN * CMD_MAX_REQ)

# define SOCK(c, id)		c->polls[id].fd
# define WRITEABLE(c, id)	(c->polls[id].revents & POLLOUT)
# define READABLE(c, id)	(c->polls[id].revents & POLLIN)

# define MAX_LEVEL			8
# define TEAM_MAX_LEN		27

/*
** Masks for setting/getting resource quantity
** 5 bits per resource for max of 31
**	TH         PH       ME         SI      DE          LI
** [THYSTAME] [PHIRAS] [MENDIANE] [SIBUR] [DERAUMERE] [LINEMATE]
*/

enum	e_resources
{
	LI,
	DE,
	SI,
	ME,
	PH,
	TH,
	NRES
};

# define RES_MAX			31
# define RES_GET(id)		(i & (0x1F << (id * 5)))
# define RES_SET(i, id, n)	(i |= (n << (id * 5)))

typedef struct	s_team
{
	char		name[TEAM_MAX_LEN + 1];
	int			members[MAX_LEVEL + 1];
	int			authorized;
}				t_team;

typedef struct	s_buff
{
	char		data[CMD_MAX_REQ][CMD_MAX_LEN];
	int			size[CMD_MAX_REQ];
	int			head;
	int			ncmds;
}				t_buff;

typedef struct	s_ent
{
	t_team		*team;
	int			level;
	int			loc_x;
	int			loc_y;
	t_ushrt		inv[NRES];
	t_buff		rbuff;
	t_buff		wbuff;
}				t_ent;

typedef struct	s_conn
{
	t_ent		*ents;
	t_poll		*polls;
	int			capacity;
	int			nsockets;
}				t_conn;

typedef struct	s_map
{
	t_uint		*data;
	int			height;
	int			width;
	size_t		size;
}				t_map;

typedef struct	s_serv
{
	t_sockin	addr;
	t_conn		conn;
	t_map		map;
	t_team		*teams;
	int			nteams;
	t_timespec	tickrate;
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
** read.c
*/

int				read_socket(t_conn *c, int id);

/*
** sockets.c
*/

int				add_socket(t_conn *c, int sock);
void			init_listener(t_serv *s);
void			remove_socket(t_conn *c, int id);

/*
** teams.c
*/

void			add_team(t_serv *s, char *name);

/*
** write.c
*/

int				send_response(t_conn *c, int id, char *msg, int len);
int				write_buffered(t_conn *c, int id);

extern const char	*g_pname;
#endif
