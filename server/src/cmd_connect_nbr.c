/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_connect_nbr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 17:06:19 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/03 19:16:37 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_connect_nbr(t_serv *s, t_team *team, int id, int dimen)
{
	t_cmd	*cmds;
	t_buff	*buff;

	cmds = GET_CMDS(s, id);
	buff = &cmds->buffs[cmds->start];
	buff->resp_len = sprintf(buff->resp, "%d\n",
		team->authorized - team->members[0]);
	if (dimen)
	{
		buff->resp_len += sprintf(buff->resp + buff->resp_len, "%d %d\n",
			s->map.width, s->map.height);
	}
}
