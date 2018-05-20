/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 03:08:47 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/20 13:21:08 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_H
# define ZAPPY_H

# define CMD_MAX_LEN	256
# define CMD_MAX_REQ	10
# define CMD_TAIL(b)	((b->head + b->ncmds) % CMD_MAX_REQ)
# define BUFF_SIZE		(CMD_MAX_LEN * CMD_MAX_REQ)

typedef struct	s_buff
{
	char		data[CMD_MAX_REQ][CMD_MAX_LEN + 1];
	int			size[CMD_MAX_REQ];
	int			head;
	int			ncmds;
}				t_buff;

typedef struct	s_conn
{
	int			sock;
	t_buff		rbuff;
	t_buff		wbuff;
}				t_conn;
#endif
