/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/27 22:04:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/29 04:50:42 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ranlib.h>
#include "ft_nm.h"

uint32_t		get_ncmds(t_obj *obj)
{
	t_mh		*head;
	t_mh64		*head_64;

	if (obj->is_64)
	{
		head_64 = (t_mh64 *)obj->pos;
		if (obj->is_rev)
			ft_revbytes(obj->pos, sizeof(t_mh64));
		obj->pos += sizeof(t_mh64);
		return (head_64->ncmds);
	}
	else
	{
		head = (t_mh *)obj->pos;
		if (obj->is_rev)
			ft_revbytes(obj->pos, sizeof(t_mh));
		obj->pos += sizeof(t_mh);
		return (head->ncmds);
	}
}

static t_bool	valid_header(t_bin *bin, t_obj *obj)
{
	uint32_t	magic;

	if (obj->pos + sizeof(uint32_t) >= bin->end)
		return (FALSE);
	magic = *(uint32_t *)obj->pos;
	if (magic == MH_MAGIC)
		;
	else if (magic == MH_CIGAM)
		obj->is_rev = TRUE;
	else if (magic == MH_MAGIC_64)
		obj->is_64 = TRUE;
	else if (magic == MH_CIGAM_64)
	{
		obj->is_64 = TRUE;
		obj->is_rev = TRUE;
	}
	else
		return (FALSE);
	if (obj->is_64)
		return (obj->pos + sizeof(t_mh64) < bin->end);
	else
		return (obj->pos + sizeof(t_mh) < bin->end);
}

static t_bool	process_object(t_bin *bin, t_obj *obj, t_bool print_text,
					t_bool multi)
{
	t_bool	res;

	obj->start = obj->pos;
	if (!valid_header(bin, obj))
	{
		if (!bin->is_ar)
		{
			NM_PERR(bin->path,
				"The file was not recognized as a valid object file");
			return (FALSE);
		}
		bin->pos += obj->ar_size;
		return (TRUE);
	}
	if (bin->is_ar)
		ft_printf("%s(%.*s):\n", bin->path, obj->namlen, obj->name);
	else if (multi)
		ft_printf("%s:\n", bin->path);
	res = (print_text) ? TRUE : print_symtab(bin, obj);
	bin->pos = (bin->is_ar) ? bin->pos + obj->ar_size : bin->end;
	return (TRUE);
}

void			process_bin(t_bin *bin, t_bool print_text, t_bool multi)
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
		write(STDOUT_FILENO, "\n", 1);
		if (!process_object(bin, obj, print_text, multi))
			break ;
		first = FALSE;
	}
}
