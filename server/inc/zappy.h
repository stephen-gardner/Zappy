/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:08:47 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/19 00:00:46 by sgardner         ###   ########.fr       */
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

# define CMD_MAX_LEN		4096
# define CMD_MAX_REQ		10
# define BUFF_SIZE			(CMD_MAX_LEN * CMD_MAX_REQ)
# define MAX_LEVEL			8
# define TEAM_MAX_LEN		27
# define HATCH_TIME			600
# define HUNGER				126

# define POLL(s, id)		(&s->conn.polls[id])
# define SOCK(s, id)		s->conn.polls[id].fd
# define READABLE(s, id)	(s->conn.polls[id].revents & POLLIN)

# define ENT(s, id)			(&s->conn.ents[id])
# define CMD_NEXT(cmd)		&(cmd)->buffs[(cmd)->start]
# define CMD_POS(cmd, i)	(((cmd)->start + i + CMD_MAX_REQ) % CMD_MAX_REQ)
# define GET_CMDS(s, id)	(&s->conn.ents[id].cmds)
# define OK(b)				b->resp_len = sprintf(b->resp, "ok\n")

# define RES_MAX			15
# define GET_LOC(s, x, y)	&s->map.data[((y * s->map.width) + x)]
# define RES_GET(r, t)		((*r & (0x0F << (t * 4))) >> (t * 4))
# define RES_SET(r, t, n)	*r = (*r & ~(0x0F << (t * 4))) | ((n) << (t * 4))
# define MOVE(a, m, n)		a = ((a + (n)) + m) % m
# define CHDIR(d, n)		(d = (d + (n) + NDIR) % NDIR)

# define SZ(x, n)			(sizeof(x) * (n))

/*
** Resource quantity map layout:
** 4 bits per resource for max quantity of 15
**	EGG      TH        PH        ME       SI        DE          LI      FOOD
** [EGG] [THYSTAME] [PHIRAS] [MENDIANE] [SIBUR] [DERAUMERE] [LINEMATE] [FOOD]
*/

enum	e_items
{
	FOOD,
	LI,
	DE,
	SI,
	ME,
	PH,
	TH,
	EGG,
	NRES
};

enum	e_dir
{
	NORTH,
	NWEST,
	WEST,
	SWEST,
	SOUTH,
	SEAST,
	EAST,
	NEAST,
	NDIR
};

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

typedef struct	s_buff
{
	uintmax_t	scheduled;
	char		recv[CMD_MAX_LEN + 1];
	char		resp[CMD_MAX_LEN + 1];
	int			recv_len;
	int			resp_len;
	int			type;
	int			pre;
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
	uintmax_t	feed_time;
}				t_ent;

typedef struct	s_conn
{
	t_ent		*ents;
	t_poll		*polls;
	int			capacity;
	int			nsockets;
	int			user_max;
}				t_conn;

typedef struct	s_egg
{
	t_team		*team;
	int			loc_x;
	int			loc_y;
	int			hatched;
	uintmax_t	scheduled;
	t_ushrt		food;
}				t_egg;

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
	t_egg		*eggs;
	t_team		*teams;
	t_map		map;
	t_timespec	tickrate;
	uintmax_t	time;
	int			neggs;
	int			nteams;
	int			go;
}				t_serv;

typedef struct	s_cmddef
{
	int			type;
	int			(*pre)(t_serv *, int, t_ent *, t_buff *);
	void		(*dispatch)(t_serv *, int, t_ent *, t_buff *);
	char		*label;
	int			len;
	int			delay;
	int			args;
}				t_cmddef;

typedef struct	s_elevreq
{
	int			nplayers;
	t_ushrt		nitems[NRES];
}				t_elevreq;

typedef struct	s_move
{
	int			loc_x;
	int			loc_y;
	int			rel_x;
	int			rel_y;
}				t_move;

/*
** cmd.c
*/

const t_cmddef	*get_cmddef(int type);
int				precmd_void(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			process_command(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			process_precommand(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** cmd_broadcast.c
*/

void			cmd_broadcast(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** cmd/cmd_connect_nbr.c
*/

void			cmd_connect(t_serv *s, int id, t_team *team, int dimen);
void			cmd_connect_nbr(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** cmd/cmd_fork.c
*/

void			cmd_fork(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** cmd/cmd_incantation.c
*/

void			cmd_incant(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				precmd_incant(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** cmd/cmd_inventory.c
*/

void			cmd_inventory(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			cmd_put(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			cmd_take(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				precmd_take(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				precmd_put(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** cmd/cmd_kick.c
*/

void			cmd_kick(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** cmd/cmd_move.c
*/

void			cmd_advance(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			cmd_left(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			cmd_right(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			move_dir(t_serv *s, t_ent *ent, int dir);

/*
** cmd/cmd_see.c
*/

void			cmd_see(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** egg.c
*/

void			add_egg(t_serv *s, t_team *team, int x, int y);
t_egg			*find_egg(t_serv *s, t_team *team);
void			incubate(t_serv *s);
void			remove_egg(t_serv *s, t_egg *egg);

/*
** map.c
*/

void			drop_stones(t_serv *s, t_ent *ent);
int				modify_resource(t_uint *loc, int type, int diff);
void			populate_map(t_serv *s);
void			replenish(t_serv *s, int type, int amount);

/*
** opt.c
*/

void			parse_opt(t_serv *s, int ac, char *const av[], char *optstr);
void			validate_opt(t_serv *s);

/*
** player.c
*/

void			level_up(t_serv *s, t_ent *ent);
int				starve_player(t_serv *s, int id);

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
t_team			*find_team(t_serv *s, char *name);
void			kill_hatchling(t_serv *s, t_egg *egg);

/*
** util.c
*/

int				get_item_id(char *name);
void			info(t_serv *s, char *fmt, ...);
t_timespec		time_diff(t_timespec t1, t_timespec t2);
void			usage_error(char *msg);

/*
** write.c
*/

int				send_message(t_serv *s, int id, char *msg, int len);
int				send_response(t_serv *s, int id);

extern const char		*g_pname;
extern const t_cmddef	g_cmddef[];
extern const int		g_cmddef_count;
extern const char		*g_items[];
extern const int		g_items_count;
#endif
