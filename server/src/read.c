/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 22:38:13 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

static void		set_eventstype(t_serv *s, t_buff *buff, int has_team)
{
	const t_evdef	*def;
	int				len;
	int				i;

	if (buff->len < EV_MAX_LEN)
	{
		i = 0;
		while (i < g_evdef_count)
		{
			def = &g_evdef[i++];
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
	t_events	*evs;
	t_buff		*buff;
	char		*dst;
	int			nl;

	evs = &ent->evs;
	buff = &evs->buffs[EV_POS(evs, evs->nevs)];
	dst = buff->data + buff->len;
	if ((nl = (sbuff[n - 1] == '\n')))
		--n;
	if ((buff->len += n) < EV_MAX_LEN)
	{
		memcpy(dst, sbuff, n);
		if (nl)
			dst[n] = '\0';
	}
	else
		*buff->data = '\0';
	if (nl)
	{
		set_eventstype(s, buff, ent->team != NULL);
		++evs->nevs;
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
	while (*start && ent->evs.nevs < EV_MAX_REQ)
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
