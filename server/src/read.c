/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 20:43:37 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

static void		set_cmdtype(t_serv *s, t_buff *buff, int has_team)
{
	const t_cmddef	*def;
	int				len;
	int				i;

	if (buff->len < CMD_MAX_LEN)
	{
		i = 0;
		while (i < g_cmddef_count)
		{
			def = &g_cmddef[i++];
			len = strlen(def->label);
			if (!strncmp(buff->data, def->label, len)
				&& *(buff->data + len) == ((def->args) ? ' ' : '\0'))
			{
				buff->type = def->type;
				return ;
			}
		}
	}
	if (has_team)
		KO(s);
}

static void		buffer_data(t_serv *s, t_ent *ent, char *sbuff, int n)
{
	t_cmd	*cmds;
	t_buff	*buff;
	char	*dst;
	int		nl;

	cmds = &ent->cmds;
	buff = &cmds->recv[CMD_POS(cmds, cmds->ncmds)];
	dst = buff->data + buff->len;
	if ((nl = (sbuff[n - 1] == '\n')))
		--n;
	if ((buff->len += n) < CMD_MAX_LEN)
	{
		memcpy(dst, sbuff, n);
		if (nl)
			dst[n] = '\0';
	}
	else
		*buff->data = '\0';
	if (nl)
	{
		set_cmdtype(s, buff, ent->team != NULL);
		++cmds->ncmds;
	}
}

int				read_socket(t_serv *s, int id, t_ent *ent)
{
	static char	sbuff[BUFF_SIZE + 1];
	char		*start;
	int			bytes;
	int			n;

	if ((bytes = read(SOCK(s, id), sbuff, BUFF_SIZE)) <= 0)
		return (-1);
	sbuff[bytes] = '\0';
	start = sbuff;
	while (*start && ent->cmds.ncmds < CMD_MAX_REQ)
	{
		n = 0;
		while (start[n] && start[n] != '\n')
			++n;
		if (start[n])
			++n;
		buffer_data(s, ent, start, n);
		start += n;
	}
	return (bytes);
}
