/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 04:58:26 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/02 17:19:20 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include "zappy.h"

int		send_message(t_serv *s, int id, char *msg, int len)
{
	return (write(SOCK(s, id), msg, len));
}

int		send_response(t_serv *s, int id)
{
	t_cmd	*cmds;
	t_buff	*buff;
	int		bytes;

	cmds = GET_CMDS(s, id);
	buff = &cmds->buffs[cmds->start];
	bytes = write(SOCK(s, id), buff->resp, buff->resp_len);
	if (bytes < 0)
		return (-1);
	if (bytes != buff->resp_len)
	{
		buff->resp_len -= bytes;
		memmove(buff->resp, buff->resp + bytes, buff->resp_len);
		return (0);
	}
	buff->recv_len = 0;
	buff->type = UNDEFINED;
	++cmds->start;
	--cmds->ncmds;
	return (0);
}
