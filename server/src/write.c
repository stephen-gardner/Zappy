/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 04:58:26 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/18 20:35:23 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

int		send_message(t_serv *s, int id, char *msg, int len)
{
	return (write(SOCK(s, id), msg, len) != len);
}

int		send_response(t_serv *s, int id)
{
	t_cmd	*cmds;
	t_buff	*buff;
	char	*pos;
	char	*nl;
	int		bytes;

	cmds = GET_CMDS(s, id);
	buff = &cmds->buffs[cmds->start];
	bytes = write(SOCK(s, id), buff->resp, buff->resp_len);
	info(s, "<%s[%s]> %s", ENT(s, id)->addr, ENT(s, id)->team, buff->recv);
	pos = buff->resp;
	while (*pos)
	{
		if (!(nl = strchr(pos, '\n')))
			break ;
		info(s, " >> %.*s", nl - pos, pos);
		pos = nl + 1;
	}
	buff->recv_len = 0;
	buff->type = UNDEFINED;
	buff->pre = 0;
	cmds->start = CMD_POS(cmds, 1);
	--cmds->ncmds;
	return (bytes != buff->resp_len);
}
