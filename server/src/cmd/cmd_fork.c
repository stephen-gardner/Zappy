/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 20:17:58 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/19 10:32:33 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_fork(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_ull	*loc;

	(void)id;
	loc = GET_LOC(s, ent->loc_x, ent->loc_y);
	modify_resource(loc, EGG, 1);
	add_egg(s, ent->team, ent->loc_x, ent->loc_y);
	OK(buff);
}
