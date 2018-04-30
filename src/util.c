/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:07:11 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/30 13:39:30 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_bool	alloc_error(void)
{
	write(STDERR_FILENO, PNAME, LEN(PNAME));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, ERRMSG, LEN(ERRMSG));
	write(STDERR_FILENO, "\n", 1);
	return (FALSE);
}

void	clean_mchains(void)
{
	t_mchain	**mchain;

	mchain = ft_mcgetall();
	while (*mchain)
	{
		while ((*mchain)->start)
			ft_mlremove((*mchain)->start);
		ft_mcdel(*mchain);
	}
}

int		nlist_cmp(t_obj *obj, t_mlink *m1, t_mlink *m2)
{
	uint64_t	a1;
	uint64_t	a2;
	int			res;

	if ((res = ft_strcmp((char *)m1->size, (char *)m2->size)))
		return (res);
	if (obj->is_64)
	{
		a1 = ((t_nlist64 *)m1->ptr)->n_value;
		a2 = ((t_nlist64 *)m2->ptr)->n_value;
	}
	else
	{
		a1 = ((t_nlist *)m1->ptr)->n_value;
		a2 = ((t_nlist *)m2->ptr)->n_value;
	}
	return (a1 > a2);
}

t_bool	truncated_obj(t_bin *bin, t_obj *obj, t_bool ret)
{
	ft_dprintf(STDERR_FILENO, "%s: %s(%.*s) %s\n", PNAME, bin->path,
		obj->namlen, obj->name, "truncated or malformed object");
	return (ret);
}
