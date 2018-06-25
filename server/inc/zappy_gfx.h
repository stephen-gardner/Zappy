/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy_gfx.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 22:40:13 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 06:34:49 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_GFX_H
# define ZAPPY_GFX_H
# include "zappy_def.h"
# include "zappy_structs.h"

/*
** #############################################################################
** # EVENT FUNCTIONS                                                           #
** #############################################################################
*/

/*
** ./events/gfx/event_bct.c
*/

void	bct(t_serv *s, int x, int y);
int		ev_bct(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/gfx/event_msz.c
*/

int		ev_msz(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/gfx/event_sgt.c
*/

int		ev_sgt(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/gfx/event_sst.c
*/

int		ev_sst(t_serv *s, int id, t_ent *ent, t_buff *buff);
void	set_tickrate(t_serv *s, int n);

/*
** ./events/gfx/event_tna.c
*/

int		ev_tna(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** #############################################################################
** # CORE FUNCTIONS                                                            #
** #############################################################################
*/

/*
** gfx.c
*/

void	add_gfx_client(t_serv *s, int id, t_ent *ent);
#endif
