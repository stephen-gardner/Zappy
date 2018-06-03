/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/02 17:40:05 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

const t_cmddef	g_cmddef[] = {
	{ NULL, 0, 0 },
	{ "advance", 7, 7 },
	{ "broadcast", 9, 7 },
	{ "connect_nbr", 11, 0 },
	{ "incantation", 11, 300 },
	{ "inventory", 9, 1 },
	{ "kick", 4, 7 },
	{ "fork", 4, 42 },
	{ "left", 4, 7 },
	{ "put", 3, 7 },
	{ "right", 5, 7 },
	{ "see", 3, 7 },
	{ "take", 4, 7 }
};

static void		set_type(t_buff *buff)
{
	const t_cmddef	*def;
	int				i;

	if (buff->recv_len < CMD_MAX_LEN)
	{
		i = 1;
		while (i < NCOMMANDS)
		{
			def = &g_cmddef[i];
			if (!strncmp(buff->recv, def->label, def->len))
			{
				buff->type = i;
				buff->delay = def->delay;
				return ;
			}
			++i;
		}
	}
	stpcpy(buff->resp, "ko\n");
	buff->resp_len = 3;
	buff->type = UNDEFINED;
	buff->delay = 0;
}

static void		buffer_data(t_cmd *cmds, char *sbuff, int n)
{
	char	*dst;
	int		idx;
	int		nl;

	idx = CMD_POS(cmds, cmds->ncmds);
	dst = cmds->buffs[idx].recv + cmds->buffs[idx].recv_len;
	if ((nl = (sbuff[n - 1] == '\n')))
		--n;
	if ((cmds->buffs[idx].recv_len += n) < CMD_MAX_LEN)
	{
		memcpy(dst, sbuff, n);
		if (nl)
			dst[n] = '\0';
	}
	else
		*cmds->buffs[idx].recv = '\0';
	if (nl)
	{
		++cmds->ncmds;
		set_type(&cmds->buffs[idx]);
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
		buffer_data(cmds, start, end - start);
		start = end;
	}
	return (bytes);
}
