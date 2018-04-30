/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:23:16 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/29 18:08:33 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_nm.h"

const char	*g_pname;

int			main(int ac, char *const av[])
{
	t_bin	bin;
	int		i;

	g_pname = av[0];
	if (ac < 2)
	{
		ft_memset(&bin, 0, sizeof(t_bin));
		bin.path = "a.out";
		load_bin(&bin);
		process_bin(&bin, FALSE, FALSE);
		return (0);
	}
	i = 1;
	while (i < ac)
	{
		ft_memset(&bin, 0, sizeof(t_bin));
		bin.path = av[i++];
		if (!load_bin(&bin))
			continue ;
		process_bin(&bin, FALSE, ac > 2);
		free(bin.data);
	}
	return (0);
}
