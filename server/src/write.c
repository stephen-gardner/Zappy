/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 04:58:26 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/04 03:28:22 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
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
	int		bytes;

	cmds = GET_CMDS(s, id);
	buff = &cmds->buffs[cmds->start];
	bytes = write(SOCK(s, id), buff->resp, buff->resp_len);
	printf("<%s> %s\n >> %.*s", ENT(s, id)->addr, buff->recv,
		(int)(strchr(buff->resp, '\n') - buff->resp + 1), buff->resp);
	buff->recv_len = 0;
	buff->type = UNDEFINED;
	cmds->start = CMD_POS(cmds, 1);
	--cmds->ncmds;
	return (bytes != buff->resp_len);
}
