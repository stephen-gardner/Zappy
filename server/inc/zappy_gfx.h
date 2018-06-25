/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy_gfx.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 22:40:13 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 03:19:01 by sgardner         ###   ########.fr       */
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
** ./events/gfx/event_sgt.c
*/

int		ev_sgt(t_serv *s, int id, t_ent *ent, t_buff *buff);

/*
** ./events/gfx/event_sst.c
*/

int		ev_sst(t_serv *s, int id, t_ent *ent, t_buff *buff);
void	set_tickrate(t_serv *s, int n);

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
