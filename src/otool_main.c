/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 08:45:01 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/03 04:34:18 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/mman.h>
#include "ft_nm.h"

const char	*g_pname;

int			main(int ac, char *const av[])
{
	t_bin	bin;
	int		i;

	g_pname = av[0];
	if (ac < 2)
	{
		ft_dprintf(STDERR_FILENO, "Usage: %s <object file> ...\n", PNAME);
		return (0);
	}
	i = 1;
	while (i < ac)
	{
		ft_memset(&bin, 0, sizeof(t_bin));
		bin.path = av[i++];
		if (!load_bin(&bin))
			continue ;
		process_bin(&bin, TRUE, ac > 2);
		munmap(bin.data, bin.end - bin.data);
	}
	return (0);
}
