/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_connect_nbr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 17:06:19 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/18 23:11:03 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_connect(t_serv *s, int id, t_team *team, int dimen)
{
	t_cmd	*cmds;
	t_buff	*buff;

	cmds = GET_CMDS(s, id);
	buff = CMD_NEXT(cmds);
	buff->resp_len = sprintf(buff->resp, "%d\n", team->authorized);
	if (!dimen)
		return ;
	buff->resp_len += sprintf(buff->resp + buff->resp_len, "%d %d\n",
		s->map.width, s->map.height);
}

void	cmd_connect_nbr(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)buff;
	cmd_connect(s, id, ent->team, 0);
}
