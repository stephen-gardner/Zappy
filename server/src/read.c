/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/04 01:37:35 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

static void		buffer_data(t_serv *s, t_cmd *cmds, char *sbuff, int n)
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
		set_cmdtype(s, &cmds->buffs[idx]);
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
