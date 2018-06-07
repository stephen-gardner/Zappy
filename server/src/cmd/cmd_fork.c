/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 20:17:58 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/05 20:21:36 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_fork(t_serv *s, int id)
{
	t_ent	*ent;
	t_buff	*buff;

	ent = ENT(s, id);
	buff = CMD_NEXT(&ent->cmds);
	++s->conn.capacity;
	++ent->team->authorized;
	buff->resp_len = sprintf(buff->resp, "ok\n");
}
