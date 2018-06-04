/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/03 00:34:00 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/04 00:33:09 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy.h"

t_timespec	time_diff(t_timespec t1, t_timespec t2)
{
	t1.tv_sec -= t2.tv_sec;
	if ((t1.tv_nsec -= t2.tv_nsec) < 0)
	{
		--t1.tv_sec;
		t1.tv_nsec += 1000000000;
	}
	return (t1);
}
