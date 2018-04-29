/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:23:16 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/29 04:47:02 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const char	*g_pname;

int			main(int ac, char *const av[])
{
	t_bin	bin;
	int		i;

	g_pname = av[0];
	i = 1;
	if (ac < 2)
	{
		ft_memset(&bin, 0, sizeof(t_bin));
		load_bin(&bin, "a.out");
		process_bin(&bin, FALSE, FALSE);
		return (0);
	}
	while (i < ac)
	{
		ft_memset(&bin, 0, sizeof(t_bin));
		if (!load_bin(&bin, av[i++]))
			continue ;
		process_bin(&bin, FALSE, TRUE);
	}
	return (0);
}
