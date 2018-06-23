/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 04:58:26 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/22 22:36:35 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"

void	build_message(t_serv *s, char *msg, ...)
{
	va_list	ap;
	char	*tmp;
	int		len;

	tmp = NULL;
	va_start(ap, msg);
	len = vasprintf(&tmp, msg, ap);
	if (!tmp
		|| !(s->resp = realloc(s->resp, s->resp_len + len + 1)))
		err(1, NULL);
	va_end(ap);
	memcpy(s->resp + s->resp_len, tmp, len);
	s->resp_len += len;
	s->resp[s->resp_len] = '\0';
	free(tmp);
}

void	send_response(t_serv *s, int id)
{
	t_ent		*ent;
	t_events	*evs;
	t_buff		*buff;
	char		*pos;
	char		*nl;

	ent = ENT(s, id);
	evs = GET_EVS(s, id);
	buff = &evs->buffs[evs->start];
	write(SOCK(s, id), s->resp, s->resp_len);
	info(s, "<%s[%s]> %s", ent->addr, ent->team, buff->data);
	pos = s->resp;
	while (pos && *pos)
	{
		if (!(nl = strchr(pos, '\n')))
			break ;
		info(s, " >> %.*s", nl - pos, pos);
		pos = nl + 1;
	}
	s->resp_len = 0;
	buff->len = 0;
	buff->pre = 0;
	buff->type = UNDEFINED;
	evs->start = EV_POS(evs, 1);
	--evs->nevs;
}
