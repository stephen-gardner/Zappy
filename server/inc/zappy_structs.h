/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 03:30:44 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/21 19:02:39 by sgardner         ###   ########.fr       */
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

typedef struct	s_team
{
	char		name[TEAM_MAX_LEN + 1];
	int			members[MAX_LEVEL + 1];
	int			authorized;
}				t_team;

typedef struct	s_ent
{
	char		addr[32];
	uuid_t		uuid;
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
	uuid_t		uuid;
	t_team		*team;
	int			loc_x;
	int			loc_y;
	int			hatched;
	uintmax_t	scheduled;
	t_ushrt		food;
}				t_egg;

typedef struct	s_map
{
	t_ull		*data;
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
}				t_serv;

/*
** #############################################################################
** # DEFINITION STRUCTS                                                        #
** #############################################################################
*/

typedef struct	s_cmddef
{
	int			type;
	int			(*pre)(t_serv *, int, t_ent *, t_buff *);
	void		(*dispatch)(t_serv *, int, t_ent *, t_buff *);
	char		*label;
	int			delay;
	int			args;
}				t_cmddef;

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
