/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_connect_nbr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 17:06:19 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/21 23:58:43 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_connect(t_serv *s, t_buff *buff, t_team *team, int dimen)
{
	build_message(buff, "%d\n", team->authorized);
	if (!dimen)
		return ;
	build_message(buff, "%d %d\n", s->map.width, s->map.height);
}

int		cmd_connect_nbr(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)id;
	cmd_connect(s, buff, ent->team, 0);
	return (0);
}
