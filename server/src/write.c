/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 04:58:26 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 01:59:40 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"

void	build_message(t_buff *buff, char *msg, ...)
{
	va_list	ap;
	char	*tmp;
	int		len;

	tmp = NULL;
	va_start(ap, msg);
	len = vasprintf(&tmp, msg, ap);
	if (!tmp
		|| !(buff->resp = realloc(buff->resp, buff->resp_len + len + 1)))
		err(1, NULL);
	va_end(ap);
	memcpy(buff->resp + buff->resp_len, tmp, len);
	buff->resp_len += len;
	buff->resp[buff->resp_len] = '\0';
	free(tmp);
}

void	send_response(t_serv *s, int id)
{
	t_cmd	*cmds;
	t_buff	*buff;
	char	*pos;
	char	*nl;

	cmds = GET_CMDS(s, id);
	buff = &cmds->buffs[cmds->start];
	write(SOCK(s, id), buff->resp, buff->resp_len);
	info(s, "<%s[%s]> %s", ENT(s, id)->addr, ENT(s, id)->team, buff->recv);
	pos = buff->resp;
	while (pos && *pos)
	{
		if (!(nl = strchr(pos, '\n')))
			break ;
		info(s, " >> %.*s", nl - pos, pos);
		pos = nl + 1;
	}
	buff->recv_len = 0;
	buff->resp_len = 0;
	buff->type = UNDEFINED;
	buff->pre = 0;
	cmds->start = CMD_POS(cmds, 1);
	--cmds->ncmds;
}
