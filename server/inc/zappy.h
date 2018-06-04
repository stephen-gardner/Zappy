/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:08:47 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/03 20:17:59 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_H
# define ZAPPY_H
# include <arpa/inet.h>
# include <err.h>
# include <poll.h>
# include <time.h>

typedef struct sockaddr		t_sock;
typedef struct sockaddr_in	t_sockin;
typedef struct pollfd		t_poll;
typedef struct timespec		t_timespec;
typedef unsigned int		t_uint;
typedef unsigned short		t_ushrt;

# define SZ(x, n)			(sizeof(x) * (n))

# define ENT(s, id)			(&s->conn.ents[id])
# define POLL(s, id)		(&s->conn.polls[id])

# define CMD_MAX_LEN		255
# define CMD_MAX_REQ		10
# define BUFF_SIZE			(CMD_MAX_LEN * CMD_MAX_REQ)
# define GET_CMDS(s, id)	(&s->conn.ents[id].cmds)
# define CMD_POS(cmd, i)	(((cmd)->start + i) % CMD_MAX_REQ)

# define SOCK(s, id)		s->conn.polls[id].fd
# define WRITABLE(s, id)	(s->conn.polls[id].revents & POLLOUT)
# define READABLE(s, id)	(s->conn.polls[id].revents & POLLIN)

# define MAX_LEVEL			8
# define TEAM_MAX_LEN		27

enum	e_dir
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

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
	FOOD,
	EGG,
	NRES
};

# define RES_MAX			15
# define RES_GET(id)		(i & (0x0F << (id * 4)))
# define RES_SET(i, id, n)	(i |= (n << (id * 4)))

typedef struct	s_team
{
	char		name[TEAM_MAX_LEN + 1];
	int			members[MAX_LEVEL + 1];
	int			authorized;
}				t_team;

enum	e_cmdtype
{
	UNDEFINED,
	ADVANCE,
	BROADCAST,
	CONNECT_NBR,
	INCANTATION,
	INVENTORY,
	KICK,
	FORK,
	LEFT,
	PUT,
	RIGHT,
	SEE,
	TAKE,
	NCOMMANDS
};

typedef struct	s_cmddef
{
	char		*label;
	int			len;
	int			delay;
}				t_cmddef;

typedef struct	s_buff
{
	char		recv[CMD_MAX_LEN + 1];
	char		resp[CMD_MAX_LEN + 1];
	int			recv_len;
	int			resp_len;
	int			type;
	int			delay;
}				t_buff;

typedef struct	s_cmd
{
	t_buff		buffs[CMD_MAX_REQ];
	int			start;
	int			ncmds;
}				t_cmd;

typedef struct	s_ent
{
	char		addr[32];
	t_cmd		cmds;
	t_team		*team;
	t_ushrt		inv[NRES];
	int			level;
	int			loc_x;
	int			loc_y;
	int			facing;
}				t_ent;

typedef struct	s_conn
{
	t_ent		*ents;
	t_poll		*polls;
	int			capacity;
	int			nsockets;
	int			user_max;
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
** cmd.c
*/

void			process_command(t_serv *s, int id);

/*
** cmd_connect_nbr.c
*/

void			cmd_connect_nbr(t_serv *s, t_team *team, int id, int dimen);

/*
** error.c
*/

void			usage_error(char *msg);

/*
** opt.c
*/

void			parse_opt(t_serv *s, int ac, char *const av[], char *optstr);
void			validate_opt(t_serv *s);

/*
** read.c
*/

int				read_socket(t_serv *s, int id);

/*
** sockets.c
*/

void			accept_incoming(t_serv *s);
int				add_socket(t_serv *s, int sock);
void			init_listener(t_serv *s);
void			remove_socket(t_serv *s, int id);

/*
** teams.c
*/

void			add_player(t_serv *s, char *name, int id);
void			add_team(t_serv *s, char *name);

/*
** util.c
*/

t_timespec		time_diff(t_timespec *t1, t_timespec *t2);

/*
** write.c
*/

int				send_message(t_serv *s, int id, char *msg, int len);
int				send_response(t_serv *s, int id);

extern const char	*g_pname;
#endif
