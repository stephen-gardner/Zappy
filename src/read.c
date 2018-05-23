/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/23 11:55:48 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

#define RBUFF(s, id)	&s->conns[id].rbuff

/*
** Copies data of valid length to client read buffer if it has space
**   Otherwise, discards data
** Increments size by bytes attempted to copy
**   Sets to 0 if end of command that was too long
** Increments ncmds if completed command is of valid length
** Returns -1 if completed command is too long, otherwise returns 0
*/

static int	buffer_data(t_buff *rbuff, char *sbuff, int n)
{
	char	*dst;
	int		idx;
	int		nl;

	idx = CMD_TAIL(rbuff);
	dst = rbuff->data[idx] + rbuff->size[idx];
	if ((nl = (sbuff[n - 1] == '\n')))
		--n;
	rbuff->size[idx] += n;
	if (rbuff->size[idx] > CMD_MAX_LEN)
	{
		if (nl)
		{
			rbuff->size[idx] = 0;
			return (-1);
		}
		return (0);
	}
	memcpy(dst, sbuff, n);
	if (nl)
		++rbuff->ncmds;
	return (0);
}

/*
** Reads data from socket into temporary buffer
** Copies data of valid length to client-specific buffer
** Returns bytes read, or -1 if an error occurs or the client disconnects
*/

int			read_socket(t_serv *s, int id)
{
	static char	sbuff[BUFF_SIZE + 1];
	t_buff		*rbuff;
	char		*start;
	char		*end;
	int			bytes;

	rbuff = RBUFF(s, id);
	if ((bytes = read(SOCK(s, id), sbuff, BUFF_SIZE)) <= 0)
		return (-1);
	sbuff[bytes] = '\0';
	start = sbuff;
	while (*start && rbuff->ncmds < CMD_MAX_REQ)
	{
		end = start;
		while (*end && *end != '\n')
			++end;
		if (buffer_data(rbuff, start, end - start))
			; // TODO: Send overflow error ?
		start = end;
	}
	return (bytes);
}
