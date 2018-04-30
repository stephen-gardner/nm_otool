/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:07:11 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/29 21:23:07 by sgardner         ###   ########.fr       */
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

void	clean_mchain(t_mchain *mchain)
{
	while (mchain->start)
		ft_mlremove(mchain->start);
	ft_mcdel(mchain);
}

t_bool	truncated_obj(t_bin *bin, t_obj *obj, t_bool ret)
{
	ft_dprintf(STDERR_FILENO, "%s: %s(%.*s) %s\n", PNAME, bin->path,
		obj->namlen, obj->name, "truncated or malformed object");
	return (ret);
}
