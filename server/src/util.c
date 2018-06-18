/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/03 00:34:00 by sgardner          #+#    #+#             */
/*   Updated: 2018/06/18 07:23:15 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zappy.h"

const char	*g_items[] = {
	"food",
	"linemate",
	"deraumere",
	"sibur",
	"mendiane",
	"phiras",
	"thystame"
};

const int	g_items_count = sizeof(g_items) / sizeof(char *);

int			get_item_id(char *name)
{
	int	i;

	i = 0;
	while (i < g_items_count)
	{
		if (!strcmp(name, g_items[i]))
			return (i);
		++i;
	}
	return (-1);
}

void		info(t_serv *s, char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	printf("[%06ju] ", s->time);
	vprintf(fmt, ap);
	printf("\n");
	va_end(ap);
}

t_timespec	time_diff(t_timespec t1, t_timespec t2)
{
	t1.tv_sec -= t2.tv_sec;
	if ((t1.tv_nsec -= t2.tv_nsec) < 0)
	{
		--t1.tv_sec;
		t1.tv_nsec += 1000000000;
	}
	return (t1);
}

void		usage_error(char *msg)
{
	const char	*short_name;

	if ((short_name = strrchr(g_pname, '/')))
		++short_name;
	else
		short_name = g_pname;
	if (msg)
		fprintf(stderr, "%s: %s\n", g_pname, msg);
	fprintf(stderr, "\nUsage: %s -n <team> [team...] [options]\n", short_name);
	fprintf(stderr, "\nOptions:\n");
	fprintf(stderr, "  -c   # authorized clients at start (default: 1)\n");
	fprintf(stderr, "  -p   port number (default: 4242)\n");
	fprintf(stderr, "  -s   seed for resource generation and spawn points\n");
	fprintf(stderr, "  -t   tick rate (default: 1)\n");
	fprintf(stderr, "  -x   world width (default: 10, min: 8)\n");
	fprintf(stderr, "  -y   world height (default: 10, min: 8)\n");
	exit(1);
}
