/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/03 00:34:00 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/03 01:00:04 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy.h"

t_timespec	time_diff(t_timespec *t1, t_timespec *t2)
{
	t_timespec	res;

	res.tv_sec = t1->tv_sec - t2->tv_sec;
	if ((res.tv_nsec = t1->tv_nsec - t2->tv_nsec) < 0)
	{
		--res.tv_sec;
		res.tv_nsec += 1000000000;
	}
	return (res);
}
