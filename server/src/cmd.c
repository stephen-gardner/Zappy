/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 18:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/02 17:41:19 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "zappy.h"

void	process_command(t_serv *s, int id)
{
	t_ent	*ent;

	ent = ENT(s, id);
	if (!ent->team)
	{
		add_player(s, ent->cmds.buffs[0].recv, id);
		send_response(s, id);
		if (!ent->team)
			remove_socket(s, id);
	}
}
