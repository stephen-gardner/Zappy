/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/05 19:07:12 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

static void		buffer_data(t_serv *s, t_cmd *cmds, char *sbuff, int n)
{
	t_buff	*buff;
	char	*dst;
	int		nl;

	buff = &cmds->buffs[CMD_POS(cmds, cmds->ncmds)];
	dst = buff->recv + buff->recv_len;
	if ((nl = (sbuff[n - 1] == '\n')))
		--n;
	if ((buff->recv_len += n) < CMD_MAX_LEN)
	{
		memcpy(dst, sbuff, n);
		if (nl)
			dst[n] = '\0';
	}
	else
		*buff->recv = '\0';
	if (nl)
	{
		buff->scheduled = (cmds->ncmds)
			? cmds->buffs[CMD_POS(cmds, cmds->ncmds - 1)].scheduled : s->time;
		set_cmdtype(buff);
		++cmds->ncmds;
	}
}

int				read_socket(t_serv *s, int id)
{
	static char	sbuff[BUFF_SIZE + 1];
	t_cmd		*cmds;
	char		*start;
	char		*end;
	int			bytes;

	cmds = GET_CMDS(s, id);
	if ((bytes = read(SOCK(s, id), sbuff, BUFF_SIZE)) <= 0)
		return (-1);
	sbuff[bytes] = '\0';
	start = sbuff;
	while (*start && cmds->ncmds < CMD_MAX_REQ)
	{
		end = start;
		while (*end && *end != '\n')
			++end;
		if (*end)
			++end;
		buffer_data(s, cmds, start, end - start);
		start = end;
	}
	return (bytes);
}
