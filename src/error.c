/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 17:54:47 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/29 12:29:57 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zappy.h"

/*
** Prints error message provided in printf format before exiting with status 1
**   If fmt is NULL, prints message from sys_errlist
*/

void	fatal_error(char *fmt, ...)
{
	va_list	ap;

	if (!fmt)
		perror(g_pname);
	else
	{
		va_start(ap, fmt);
		fprintf(stderr, "%s: ", g_pname);
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
		va_end(ap);
	}
	exit(1);
}

/*
** Prints msg if not NULL, then usage
*/

void	usage_error(char *msg)
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
	fprintf(stderr, "  -x   world width (default: 10)\n");
	fprintf(stderr, "  -y   world height (default: 10)\n");
	exit(1);
}
