/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_connect_nbr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 17:06:19 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/04 19:36:17 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_connect_nbr(t_serv *s, t_team *team, int id, int dimen)
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
