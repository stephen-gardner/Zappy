/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 01:21:56 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 01:38:03 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy.h"

void	add_gfx_client(t_serv *s, int id, t_ent *ent)
{
	ent->type = ENT_GRAPHIC;
	send_response(s, id);
}
