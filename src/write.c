/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 04:58:26 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/20 13:35:46 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "zappy.h"

/*
** Writes data from client buffers to socket, starting with oldest command.
** If socket becomes full, moves remainder of current command to the front of
**  its buffer and updates its size
** If a full command is written, sets its buffer size to 0 and decrements ncmds
** Returns total bytes written to socket, or -1 if an error occurs
*/

int		write_socket(t_conn *conn)
{
	t_buff	*cbuff;
	int		idx;
	int		bytes;
	int		total;

	total = 0;
	cbuff = &conn->wbuff;
	while (cbuff->size[(idx = cbuff->head)])
	{
		if ((bytes = write(conn->sock, cbuff->data, cbuff->size[idx])) < 0)
			return (-1);
		total += bytes;
		if (bytes < cbuff->size[idx])
		{
			cbuff->size[idx] -= bytes;
			memmove(cbuff->data, cbuff->data + bytes, cbuff->size[idx]);
			break ;
		}
		--cbuff->ncmds;
		cbuff->size[idx] = 0;
		cbuff->head = (cbuff->head + 1) % CMD_MAX_REQ;
	}
	return (total);
}
