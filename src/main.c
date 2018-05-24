/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/21 05:50:28 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/24 11:17:29 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zappy.h"

const char	*g_pname;

int			main(int ac, char *const av[])
{
	static t_serv	s;

	g_pname = av[0];
	memset(&s, 0, sizeof(t_serv));
	s.opt.addr.sin_port = htons(4242);
	parse_options(&s, ac, av);
	if (!s.opt.nteams)
		usage_error("No teams specified");
	printf("%s starting...\n", g_pname);
	if (!(s.conns = calloc(s.capacity, sizeof(t_conn))))
		FATAL(NULL);
	if (!(s.polls = calloc(s.capacity, sizeof(t_poll))))
		FATAL(NULL);
	init_listener(&s);
	return (0);
}
