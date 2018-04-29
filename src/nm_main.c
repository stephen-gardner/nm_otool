/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:23:16 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/28 14:08:48 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ranlib.h>
#include "ft_nm.h"

const char	*g_pname;

static void	process_bin(t_bin *bin)
{
	t_obj		*obj;
	t_bool		first;

	first = TRUE;
	while (bin->pos < bin->end)
	{
		if (!(obj = get_next_obj(bin)))
			break ;
		if (first && bin->is_ar &&
			((!ft_memcmp(obj->name, SYMDEF, obj->namlen)
			|| !ft_memcmp(obj->name, SYMDEF_SORTED, obj->namlen))))
		{
			bin->pos += obj->ar_size;
			continue ;
		}
		if (!process_object(bin, obj))
			break ;
		first = FALSE;
	}
}

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
		process_bin(&bin);
	}
	while (i < ac)
	{
		ft_memset(&bin, 0, sizeof(t_bin));
		if (!load_bin(&bin, av[i++]))
			continue ;
		process_bin(&bin);
	}
	return (0);
}
