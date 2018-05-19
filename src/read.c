/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:32:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/19 08:24:44 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

/*
** Copies data of valid length to client read buffer if it has space
**   Otherwise, discards data
** Increments ncmds if completed command is of valid length
** Increments frag_len by bytes attempted to copy
**   Sets to 0 if end of command
** Returns -1 if completed command is too long
*/

static int	buffer_data(t_buff *cbuff, char *sbuff, int n)
{
	char	*dst;
	int		err;
	int		nl;

	dst = cbuff->data[CMD_IDX(cbuff->start, cbuff->ncmds)] + cbuff->frag_len;
	err = (cbuff->frag_len + n > CMD_MAX_LEN);
	if (!err)
		memcpy(dst, sbuff, n);
	if ((nl = sbuff[n - 1] == '\n'))
	{
		if (!err)
		{
			dst[n - 1] = '\0';
			++cbuff->ncmds;
		}
		cbuff->frag_len = 0;
	}
	else
		cbuff->frag_len += n;
	return ((err && nl) ? -1 : 0);
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
