/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/21 05:50:28 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/23 11:57:18 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "zappy.h"

void	fatal_error(void)
{
	perror(PNAME);
	exit(1);
}

int		main(int ac, char **av)
{
	static t_serv	*s;

	(void)ac;
	(void)av;
	(void)s;
	return (0);
}
