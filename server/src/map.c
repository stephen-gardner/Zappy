/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 17:51:05 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/07 02:01:55 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy.h"

t_uint	get_res(t_serv *s, t_loc loc, int type)
{
	size_t	pos;

	pos = (loc.y * s->map.width) + loc.x;
	return ((s->map.data[pos] & (0x0F << (type * 4))) >> (type * 4));
}

void	set_res(t_serv *s, t_loc loc, int type, t_uint n)
{
	size_t	pos;

	pos = (loc.y * s->map.width) + loc.x;
	s->map.data[pos] &= ~(0x0F << (type * 4));
	s->map.data[pos] |= n << (type * 4);
}
