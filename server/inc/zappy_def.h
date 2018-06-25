/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy_def.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 03:24:13 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 03:28:38 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_DEF_H
# define ZAPPY_DEF_H
# include <arpa/inet.h>
# include <err.h>
# include <poll.h>
# include <time.h>

/*
** #############################################################################
** # TYPEDEFS                                                                  #
** #############################################################################
*/

typedef struct sockaddr		t_sock;
typedef struct sockaddr_in	t_sockin;
typedef struct pollfd		t_poll;
typedef struct timespec		t_timespec;
typedef unsigned long long	t_ull;
typedef unsigned short		t_ushrt;

/*
** #############################################################################
** # CONFIG                                                                    #
** #############################################################################
*/

# define EV_MAX_LEN			255
# define EV_MAX_REQ			10
# define MAX_LEVEL			8
# define REQ_MAX_PLAYERS	6
# define TEAM_MAX_LEN		27
# define HATCH_TIME			600
# define HUNGER				126
# define TIMEOUT			5

# define ADV_DEL			7
# define BRO_DEL			7
# define CON_DEL			0
# define INC_DEL			300
# define INV_DEL			1
# define KIC_DEL			7
# define FOR_DEL			42
# define LEF_DEL			7
# define PUT_DEL			7
# define RIG_DEL			7
# define SEE_DEL			7
# define TAK_DEL			7

# define SGT_DEL			0
# define SST_DEL			0

/*
** #############################################################################
** # LANG                                                                      #
** #############################################################################
*/

# define WELCOME			"WELCOME\n"
# define ELEVATING			"elevation in progress\n"
# define CURR_LEVEL			"current level : %d\n"
# define DEATH				"death\n"
# define PLAYER				"player"
# define FOOD_STR			"food"

# define ADV_STR			"advance"
# define BRO_STR			"broadcast"
# define CON_STR			"connect_nbr"
# define INC_STR			"incantation"
# define INV_STR			"inventory"
# define KIC_STR			"kick"
# define FOR_STR			"fork"
# define LEF_STR			"left"
# define PUT_STR			"put"
# define RIG_STR			"right"
# define SEE_STR			"see"
# define TAK_STR			"take"

# define AUTH_GRAPHIC		"GRAPHIC"

# define SGT_STR			"Sgt"
# define SST_STR			"sst"

/*
** # define WELCOME			"BIENVENUE\n"
** # define ELEVATING			"elevation en cours\n"
** # define CURR_LEVEL			"niveau actuel : %d\n"
** # define DEATH				"mort\n"
** # define PLAYER				"joueur"
** # define FOOD_STR			"nourriture"
**
** # define ADV_STR			"avance"
** # define BRO_STR			"broadcast"
** # define CON_STR			"connect_nbr"
** # define INC_STR			"incantation"
** # define INV_STR			"inventaire"
** # define KIC_STR			"expulse"
** # define FOR_STR			"fork"
** # define LEF_STR			"gauche"
** # define PUT_STR			"pose"
** # define RIG_STR			"droite"
** # define SEE_STR			"voir"
** # define TAK_STR			"prend"
*/

/*
** #############################################################################
** # UTILITY MACROS                                                            #
** #############################################################################
*/

# define POLL(s, id)		(&s->conn.polls[id])
# define SOCK(s, id)		s->conn.polls[id].fd
# define READABLE(s, id)	(s->conn.polls[id].revents & POLLIN)

# define ENT(s, id)			(&s->conn.ents[id])
# define EV_NEXT(ev)		(&(ev)->buffs[(ev)->start])
# define EV_POS(ev, i)		(((ev)->start + i + EV_MAX_REQ) % EV_MAX_REQ)
# define GET_EVS(s, id)		(&s->conn.ents[id].evs)
# define OK(s)				build_message(s, "ok\n")
# define KO(s)				build_message(s, "ko\n")

# define MOVE(a, m, n)		a = ((a + (n)) + m) % m
# define CHDIR(d, n)		(d = (d + (n) + NDIR) % NDIR)

/*
** Resource quantity map layout:
** 1 byte per resource for max quantity of 255
**	EGG      TH        PH        ME       SI        DE          LI      FOOD
** [EGG] [THYSTAME] [PHIRAS] [MENDIANE] [SIBUR] [DERAUMERE] [LINEMATE] [FOOD]
*/

# define RES_MAX			255
# define GET_LOC(s, x, y)	&s->map.data[((y * s->map.width) + x)]
# define RES_GET(r, t)		((unsigned char *)r)[t]
# define RES_SET(r, t, n)	((unsigned char *)r)[t] = n

# define SZ(x, n)			(sizeof(x) * (n))
# define BUFF_SIZE			(EV_MAX_LEN * EV_MAX_REQ)

/*
** #############################################################################
** # ENUM CONSTANTS                                                            #
** #############################################################################
*/

enum	e_enttype
{
	ENT_UNDEFINED,
	ENT_PLAYER,
	ENT_GRAPHIC,
	NENTTYPES
};

enum	e_evtype
{
	UNDEFINED,
	EV_ADV,
	EV_BRO,
	EV_CON,
	EV_INC,
	EV_INV,
	EV_KIC,
	EV_FOR,
	EV_LEF,
	EV_PUT,
	EV_RIG,
	EV_SEE,
	EV_TAK,
	EV_SGT,
	EV_SST,
	NEVENTS
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
