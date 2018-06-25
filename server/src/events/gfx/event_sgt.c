/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_sgt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 02:20:52 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 02:47:02 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

int	ev_sgt(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	(void)id;
	(void)ent;
	(void)buff;
	build_message(s, "sgt %d\n", s->ticks);
	return (0);
}
