/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/19 07:28:57 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"

static void		set_cmdtype(t_ent *ent, t_buff *buff)
{
	const t_cmddef	*def;
	int				i;

	buff->resp_len = sprintf(buff->resp, "ko\n");
	if (buff->recv_len < CMD_MAX_LEN)
	{
		i = 0;
		while (i < g_cmddef_count)
		{
			def = &g_cmddef[i++];
			if (!strncmp(buff->recv, def->label, def->len)
				&& *(buff->recv + def->len) == ((def->args) ? ' ' : '\0'))
			{
				if ((buff->type = def->type) == INCANTATION)
				{
					buff->resp_len = sprintf(buff->resp, "current level : %d\n",
						ent->level);
				}
				return ;
			}
		}
	}
}

static void		buffer_data(t_ent *ent, char *sbuff, int n)
{
	t_cmd	*cmds;
	t_buff	*buff;
	char	*dst;
	int		nl;

	cmds = &ent->cmds;
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
		set_cmdtype(ent, buff);
		++cmds->ncmds;
	}
}

int				read_socket(t_ent *ent, int sock)
{
	static char	sbuff[BUFF_SIZE + 1];
	char		*start;
	char		*end;
	int			bytes;

	if ((bytes = read(sock, sbuff, BUFF_SIZE)) <= 0)
		return (-1);
	sbuff[bytes] = '\0';
	start = sbuff;
	while (*start && ent->cmds.ncmds < CMD_MAX_REQ)
	{
		end = start;
		while (*end && *end != '\n')
			++end;
		if (*end)
			++end;
		buffer_data(ent, start, end - start);
		start = end;
	}
	return (bytes);
}
