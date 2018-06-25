/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_tna.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 04:17:44 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 05:24:34 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy.h"

int	ev_tna(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	int		i;

	(void)id;
	(void)ent;
	(void)buff;
	i = 0;
	while (i < s->nteams)
		build_message(s, "tna %s\n", s->teams[i++].name);
	return (0);
}
