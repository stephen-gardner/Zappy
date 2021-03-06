/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 03:30:44 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 07:31:54 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_STRUCTS_H
# define ZAPPY_STRUCTS_H

/*
** #############################################################################
** # CORE STRUCTS                                                              #
** #############################################################################
*/

typedef struct	s_buff
{
	char		data[EV_MAX_LEN + 1];
	uintmax_t	scheduled;
	int			len;
	int			pre;
	int			type;
}				t_buff;

typedef struct	s_events
{
	t_buff		buffs[EV_MAX_REQ];
	int			start;
	int			nevs;
}				t_events;

typedef struct	s_team
{
	char		name[TEAM_MAX_LEN + 1];
	int			members[MAX_LEVEL + 1];
	int			authorized;
}				t_team;

typedef struct	s_ent
{
	t_events	evs;
	intmax_t	uid;
	char		addr[32];
	t_team		*team;
	t_ushrt		inv[NRES];
	uintmax_t	feed_time;
	int			level;
	int			loc_x;
	int			loc_y;
	int			facing;
	int			type;
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
	intmax_t	uid;
	uintmax_t	scheduled;
	t_team		*team;
	int			loc_x;
	int			loc_y;
	int			hatched;
	t_ushrt		food;
}				t_egg;

typedef struct	s_map
{
	t_ull		*data;
	size_t		size;
	int			height;
	int			width;
}				t_map;

typedef struct	s_serv
{
	t_conn		conn;
	t_map		map;
	t_sockin	addr;
	t_timespec	tickrate;
	intmax_t	uid;
	uintmax_t	time;
	t_egg		*eggs;
	t_team		*teams;
	char		*resp;
	int			resp_len;
	int			neggs;
	int			nteams;
	int			ticks;
}				t_serv;

/*
** #############################################################################
** # DEFINITION STRUCTS                                                        #
** #############################################################################
*/

typedef struct	s_eventsdef
{
	int			evtype;
	int			enttype;
	int			(*pre)(t_serv *, int, t_ent *, t_buff *);
	int			(*dispatch)(t_serv *, int, t_ent *, t_buff *);
	char		*label;
	int			delay;
	int			args;
}				t_evdef;

typedef struct	s_elevreq
{
	int			nplayers;
	t_ushrt		nitems[NRES];
}				t_elevreq;

/*
** #############################################################################
** # UTILITY STRUCTS                                                           #
** #############################################################################
*/

typedef struct	s_move
{
	int			loc_x;
	int			loc_y;
	int			rel_x;
	int			rel_y;
}				t_move;
#endif
