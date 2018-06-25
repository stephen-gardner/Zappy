/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_connect_nbr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 17:06:19 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 22:43:10 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	ev_connect(t_serv *s, t_team *team, int dimen)
{
	build_message(s, "%d\n", team->authorized);
	if (!dimen)
		return ;
	build_message(s, "%d %d\n", s->map.width, s->map.height);
}

int		ev_connect_nbr(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)id;
	(void)buff;
	ev_connect(s, ent->team, 0);
	return (0);
}
