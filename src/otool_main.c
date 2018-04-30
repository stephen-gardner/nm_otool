/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 08:45:01 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/30 10:28:11 by sgardner         ###   ########.fr       */
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
		return (0);
	i = 1;
	while (i < ac)
	{
		ft_memset(&bin, 0, sizeof(t_bin));
		bin.path = av[i++];
		if (!load_bin(&bin))
			continue ;
		process_bin(&bin, TRUE, ac > 2);
		free(bin.data);
	}
	return (0);
}
