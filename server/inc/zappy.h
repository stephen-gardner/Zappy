/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:08:47 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/21 23:55:51 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_H
# define ZAPPY_H
# include <arpa/inet.h>
# include <err.h>
# include <poll.h>
# include <time.h>
# include <uuid/uuid.h>
# include "zappy_def.h"
# include "zappy_structs.h"

/*
** #############################################################################
** # COMMAND FUNCTIONS                                                         #
** #############################################################################
*/

/*
** ./cmd/cmd.c
*/

const t_cmddef	*get_cmddef(int type);
int				precmd_void(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			process_command(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			process_precommand(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./cmd/cmd_broadcast.c
*/

int				cmd_broadcast(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./cmd/cmd_connect_nbr.c
*/

void			cmd_connect(t_serv *s, t_buff *buff, t_team *team, int dimen);
int				cmd_connect_nbr(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./cmd/cmd_fork.c
*/

int				cmd_fork(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./cmd/cmd_incantation.c
*/

int				cmd_incant(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				precmd_incant(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./cmd/cmd_inventory.c
*/

int				cmd_inventory(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				cmd_put(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				cmd_take(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				precmd_take(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				precmd_put(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./cmd/cmd_kick.c
*/

int				cmd_kick(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./cmd/cmd_move.c
*/

int				cmd_advance(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				cmd_left(t_serv *s, int id, t_ent *ent, t_buff *buff);
int				cmd_right(t_serv *s, int id, t_ent *ent, t_buff *buff);
void			move_dir(t_serv *s, t_ent *ent, int dir);

/*
** ./cmd/cmd_see.c
*/

int				cmd_see(t_serv *s, int id, t_ent *ent, t_buff *buff);

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

void			end_game(t_serv *s, t_team *team);
void			level_up(t_serv *s, t_ent *ent, t_buff *buff);
int				starve_player(t_serv *s, t_ent *ent);

/*
** read.c
*/

int				read_socket(t_ent *ent, int sock);

/*
** sockets.c
*/

void			accept_incoming(t_serv *s);
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

void			build_message(t_buff *buff, char *msg, ...);
void			send_response(t_serv *s, int id);

/*
** #############################################################################
** # CONSTANTS                                                                 #
** #############################################################################
*/

extern const char		*g_pname;
extern const t_cmddef	g_cmddef[];
extern const int		g_cmddef_count;
extern const char		*g_items[];
extern const int		g_items_count;
#endif
