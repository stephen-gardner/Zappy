/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy_def.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 03:24:13 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/19 20:56:28 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_DEF_H
# define ZAPPY_DEF_H

/*
** #############################################################################
** # TYPEDEFS                                                                  #
** #############################################################################
*/

typedef struct sockaddr		t_sock;
typedef struct sockaddr_in	t_sockin;
typedef struct pollfd		t_poll;
typedef struct timespec		t_timespec;
typedef unsigned int		t_uint;
typedef unsigned short		t_ushrt;

/*
** #############################################################################
** # CONFIG                                                                    #
** #############################################################################
*/

# define CMD_MAX_LEN		4096
# define CMD_MAX_REQ		10
# define MAX_LEVEL			8
# define TEAM_MAX_LEN		27
# define HATCH_TIME			600
# define HUNGER				126

/*
** #############################################################################
** # COMMANDS                                                                  #
** #############################################################################
*/

# define ADV_STR			"advance"
# define ADV_DEL			7

# define BRO_STR			"broadcast"
# define BRO_DEL			7

# define CON_STR			"connect_nbr"
# define CON_DEL			0

# define INC_STR			"incantation"
# define INC_DEL			300

# define INV_STR			"inventory"
# define INV_DEL			1

# define KIC_STR			"kick"
# define KIC_DEL			7

# define FOR_STR			"fork"
# define FOR_DEL			42

# define LEF_STR			"left"
# define LEF_DEL			7

# define PUT_STR			"put"
# define PUT_DEL			7

# define RIG_STR			"right"
# define RIG_DEL			7

# define SEE_STR			"see"
# define SEE_DEL			7

# define TAK_STR			"take"
# define TAK_DEL			7

/*
** #############################################################################
** # UTILITY MACROS                                                            #
** #############################################################################
*/

# define POLL(s, id)		(&s->conn.polls[id])
# define SOCK(s, id)		s->conn.polls[id].fd
# define READABLE(s, id)	(s->conn.polls[id].revents & POLLIN)

# define ENT(s, id)			(&s->conn.ents[id])
# define CMD_NEXT(cmd)		&(cmd)->buffs[(cmd)->start]
# define CMD_POS(cmd, i)	(((cmd)->start + i + CMD_MAX_REQ) % CMD_MAX_REQ)
# define GET_CMDS(s, id)	(&s->conn.ents[id].cmds)
# define OK(b)				b->resp_len = sprintf(b->resp, "ok\n")

# define MOVE(a, m, n)		a = ((a + (n)) + m) % m
# define CHDIR(d, n)		(d = (d + (n) + NDIR) % NDIR)

/*
** Resource quantity map layout:
** 4 bits per resource for max quantity of 15
**	EGG      TH        PH        ME       SI        DE          LI      FOOD
** [EGG] [THYSTAME] [PHIRAS] [MENDIANE] [SIBUR] [DERAUMERE] [LINEMATE] [FOOD]
*/

# define RES_MAX			15
# define GET_LOC(s, x, y)	&s->map.data[((y * s->map.width) + x)]
# define RES_GET(r, t)		((*r & (0x0F << (t * 4))) >> (t * 4))
# define RES_SET(r, t, n)	*r = (*r & ~(0x0F << (t * 4))) | ((n) << (t * 4))

# define SZ(x, n)			(sizeof(x) * (n))
# define BUFF_SIZE			(CMD_MAX_LEN * CMD_MAX_REQ)

/*
** #############################################################################
** # ENUM CONSTANTS                                                            #
** #############################################################################
*/

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
#endif
