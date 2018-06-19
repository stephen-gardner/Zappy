/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/18 21:30:39 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"

static int		cmdcmp(char *cmd, const t_cmddef *def)
{
	if (strncmp(cmd, def->label, def->len))
		return (1);
	return (*(cmd + def->len) != ((def->args) ? ' ' : '\0'));
}

static void		set_cmdtype(t_serv *s, int id)
{
	t_cmd			*cmds;
	t_buff			*buff;
	const t_cmddef	*def;
	int				i;

	cmds = GET_CMDS(s, id);
	buff = &cmds->buffs[CMD_POS(cmds, cmds->ncmds)];
	buff->resp_len = sprintf(buff->resp, "ko\n");
	if (buff->recv_len < CMD_MAX_LEN)
	{
		i = 0;
		while (i < g_cmddef_count)
		{
			def = &g_cmddef[i++];
			if (!cmdcmp(buff->recv, def))
			{
				if ((buff->type = def->type) == INCANTATION)
				{
					buff->resp_len = sprintf(buff->resp, "current level : %d\n",
						ENT(s, id)->level);
				}
				return ;
			}
		}
	}
}

static void		buffer_data(t_serv *s, int id, char *sbuff, int n)
{
	t_cmd	*cmds;
	t_buff	*buff;
	char	*dst;
	int		nl;

	cmds = GET_CMDS(s, id);
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
		set_cmdtype(s, id);
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
		buffer_data(s, id, start, end - start);
		start = end;
	}
	return (bytes);
}
