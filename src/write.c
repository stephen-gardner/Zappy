/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 04:58:26 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/23 11:55:50 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

#define WBUFF(s, id)	&s->conns[id].wbuff

/*
** Buffers data
** Returns # bytes buffered, or -1 if max # requests are already buffered
*/

static int	buffer_data(t_buff *wb, char *msg, int len)
{
	int		idx;

	if (wb->ncmds == CMD_MAX_REQ)
		return (-1);
	idx = CMD_TAIL(wb);
	memcpy(wb->data[idx], msg, len);
	wb->size[idx] = len;
	++wb->ncmds;
	return (len);
}

/*
** Writes data to socket
** Buffers whatever can't be written
** Returns # bytes unwritten, 0 if completed, or -1 if an error occurs
*/

int			send_response(t_serv *s, int id, char *msg, int len)
{
	int	bytes;

	if (WRITEABLE(s, id))
	{
		if ((bytes = write(SOCK(s, id), msg, len)) < 0)
			return (-1);
		if (bytes == len)
			return (0);
		msg += bytes;
		len -= bytes;
	}
	return (buffer_data(WBUFF(s, id), msg, len));
}

/*
** Writes data from client buffers to socket, starting with oldest command.
** If socket becomes full, moves remainder of current command to the front of
**  its buffer and updates its size
** If a full command is written, sets its buffer size to 0 and decrements ncmds
** Returns ncmds if data remains buffered, or -1 if an error occurs
*/

int			write_buffered(t_serv *s, int id)
{
	t_buff	*wbuff;
	int		idx;
	int		bytes;

	wbuff = WBUFF(s, id);
	while (wbuff->size[(idx = wbuff->head)])
	{
		if ((bytes = write(SOCK(s, id), wbuff->data, wbuff->size[idx])) < 0)
			return (-1);
		if (bytes < wbuff->size[idx])
		{
			wbuff->size[idx] -= bytes;
			memmove(wbuff->data, wbuff->data + bytes, wbuff->size[idx]);
			return (wbuff->ncmds);
		}
		--wbuff->ncmds;
		wbuff->size[idx] = 0;
		wbuff->head = (wbuff->head + 1) % CMD_MAX_REQ;
	}
	return (0);
}
