/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_connect_nbr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 17:06:19 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/29 20:25:41 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	cmd_connect_nbr(t_serv *s, t_team *team, int id)
{
	t_buff	*rbuff;
	char	resp[32];
	int		len;

	rbuff = &s->conn.ents[id].rbuff;
	len = sprintf(resp, "%d\n%d %d\n", team->authorized - team->members[0],
		s->map.width, s->map.height);
	send_response(&s->conn, id, resp, len);
	rbuff->size[CMD_POS(rbuff, 0)] = 0;
	rbuff->head = CMD_POS(rbuff, 1);
	--rbuff->ncmds;
}
