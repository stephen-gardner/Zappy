/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_ppo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 07:14:03 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/25 08:07:28 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "zappy.h"

void	ppo(t_serv *s, t_ent *ent)
{
	build_message(s, "ppo %jd %d %d ", ent->uid, ent->loc_x, ent->loc_y);
	if (ent->facing == NORTH)
		build_message(s, "%d", 1);
	else if (ent->facing == EAST)
		build_message(s, "%d", 2);
	else if (ent->facing == SOUTH)
		build_message(s, "%d", 3);
	else
		build_message(s, "%d", 4);
}

int		ev_ppo(t_serv *s, int id, t_ent *ent, t_buff *buff)
{
	t_ent		*player;
	intmax_t	uid;

	(void)id;
	(void)ent;
	if ((uid = parse_uid(strchr(buff->data, ' ') + 1)) == -1
		|| !(player = find_player(s, uid)))
		return (-1);
	ppo(s, player);
	build_message(s, "\n");
	return (0);
}
