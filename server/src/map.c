/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 17:51:05 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/11 21:08:09 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy.h"

int	modify_resource(t_uint *loc, int type, int diff)
{
	int	n;

	if (!diff)
		return (0);
	n = RES_GET(loc, type);
	n += diff;
	if (n < 0 || n > RES_MAX)
		return (-1);
	RES_SET(loc, type, n);
	return (0);
}
