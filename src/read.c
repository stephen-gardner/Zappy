/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/20 13:21:00 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

/*
** Copies data of valid length to client read buffer if it has space
**   Otherwise, discards data
** Increments size by bytes attempted to copy
**   Sets to 0 if end of command that was too long
** Increments ncmds if completed command is of valid length
** Returns -1 if completed command is too long, otherwise returns 0
*/

static int	buffer_data(t_buff *cbuff, char *sbuff, int n)
{
	char	*dst;
	int		idx;
	int		nl;

	idx = CMD_TAIL(cbuff);
	dst = cbuff->data[idx] + cbuff->size[idx];
	if ((nl = (sbuff[n - 1] == '\n')))
		--n;
	cbuff->size[idx] += n;
	if (cbuff->size[idx] > CMD_MAX_LEN)
	{
		if (nl)
		{
			cbuff->size[idx] = 0;
			return (-1);
		}
		return (0);
	}
	memcpy(dst, sbuff, n);
	if (nl)
		++cbuff->ncmds;
	return (0);
}

/*
** Reads data from socket into temporary buffer
** Copies data of valid length to client-specific buffer
** Returns bytes read, or -1 if an error occurs or the client disconnects
*/

int			read_socket(t_conn *conn)
{
	static char	sbuff[BUFF_SIZE + 1];
	t_buff		*cbuff;
	char		*start;
	char		*end;
	int			bytes;

	cbuff = &conn->rbuff;
	if ((bytes = read(conn->sock, sbuff, BUFF_SIZE)) <= 0)
		return (-1);
	sbuff[bytes] = '\0';
	start = sbuff;
	while (*start && cbuff->ncmds < CMD_MAX_REQ)
	{
		end = start;
		while (*end && *end != '\n')
			++end;
		if (buffer_data(cbuff, start, end - start))
			; // TODO: Send overflow error ?
		start = end;
	}
	return (bytes);
}
