/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_msz.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 03:59:01 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 04:12:17 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy.h"

int	ev_msz(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)id;
	(void)ent;
	(void)buff;
	build_message(s, "msz %d %d\n", s->map.width, s->map.height);
	return (0);
}
