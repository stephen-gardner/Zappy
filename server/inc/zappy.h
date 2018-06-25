/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:08:47 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 02:26:07 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_H
# define ZAPPY_H
# include "zappy_def.h"
# include "zappy_structs.h"
# include "zappy_gfx.h"

/*
** #############################################################################
** # EVENT FUNCTIONS                                                           #
** #############################################################################
*/

/*
** ./events/event.c
*/

const t_evdef	*get_evdef(int enttype, int evtype);
int				preev_void(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			process_command(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			process_precommand(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/ai/event_broadcast.c
*/

int				ev_broadcast(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/ai/event_connect_nbr.c
*/

void			ev_connect(t_serv *s, t_team *team, int dimen);
int				ev_connect_nbr(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/ai/event_fork.c
*/

int				ev_fork(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/ai/event_incantation.c
*/

int				ev_incant(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				preev_incant(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/ai/event_inventory.c
*/

int				ev_inventory(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				ev_put(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				ev_take(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				preev_take(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				preev_put(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/ai/event_kick.c
*/

int				ev_kick(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/ai/event_move.c
*/

int				ev_advance(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				ev_left(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				ev_right(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			move_dir(t_serv *s, t_ent *ent, int dir);

/*
** ./events/ai/event_see.c
*/

int				ev_see(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** #############################################################################
** # CORE FUNCTIONS                                                            #
** #############################################################################
*/

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
int				modify_resource(t_ull *loc, int type, int diff);
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

int				count_players(t_serv *s);
void			end_game(t_serv *s, t_team *team);
void			level_up(t_serv *s, t_ent *ent);
int				starve_player(t_serv *s, t_ent *ent);

/*
** read.c
*/

int				read_socket(t_serv *s, int id, t_ent *ent);

/*
** sockets.c
*/

void			accept_incoming(t_serv *s);
void			init_listener(t_serv *s);
void			remove_socket(t_serv *s, int id);

/*
** teams.c
*/

void			add_player(t_serv *s, int id, t_ent *ent, t_buff *buff);
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

void			build_message(t_serv *s, char *msg, ...);
void			send_response(t_serv *s, int id);

/*
** #############################################################################
** # CONSTANTS                                                                 #
** #############################################################################
*/

extern const char		*g_pname;
extern const t_evdef	g_evdef[];
extern const int		g_evdef_count;
extern const char		*g_items[];
extern const int		g_items_count;
#endif
