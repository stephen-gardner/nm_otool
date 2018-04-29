/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/27 22:04:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/28 21:42:54 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static t_bool		valid_header(t_bin *bin, t_obj *obj)
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

static uint32_t		get_ncmds(t_obj *obj)
{
	t_mh		*head;
	t_mh64		*head_64;
	uint32_t	ncmds;

	if (obj->is_64)
	{
		head_64 = (t_mh64 *)obj->pos;
		if (obj->is_rev)
			ft_revbytes(obj->pos, sizeof(t_mh64));
		ncmds = head_64->ncmds;
		obj->pos += sizeof(t_mh64);
	}
	else
	{
		head = (t_mh *)obj->pos;
		if (obj->is_rev)
			ft_revbytes((t_byte *)obj->pos, sizeof(t_mh));
		ncmds = head->ncmds;
		obj->pos += sizeof(t_mh);
	}
	return (ncmds);
}

static t_stabcmd	*find_symtab(t_bin *bin, t_obj *obj)
{
	t_lc		*lc;
	t_stabcmd	*symtab;
	uint32_t	ncmds;
	uint32_t	i;

	i = 0;
	ncmds = get_ncmds(obj);
	while (i++ < ncmds)
	{
		lc = (t_lc *)obj->pos;
		if ((obj->pos += lc->cmdsize) >= bin->end)
			break ;
		if (obj->is_rev)
			ft_revbytes((t_byte *)lc, sizeof(t_lc));
		if (lc->cmd != LC_SYMTAB)
			continue ;
		symtab = (t_stabcmd *)lc;
		if (obj->is_rev)
		{
			ft_revbytes((t_byte *)symtab + sizeof(t_lc),
				sizeof(t_stabcmd) - sizeof(t_lc));
		}
		return (symtab);
	}
	return (NULL);
}

static void			print_symtab(t_stabcmd *symtab)
{
	ft_printf("% .16lx\n", 0);
	UNUSED(symtab);
}

t_bool				process_object(t_bin *bin, t_obj *obj)
{
	t_stabcmd	*symtab;

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
	if (obj->name)
		ft_printf("\n%s(%.*s):\n", bin->path, obj->namlen, obj->name);
	if ((symtab = find_symtab(bin, obj)))
		print_symtab(symtab);
	bin->pos = (bin->is_ar) ? bin->pos + obj->ar_size : bin->end;
	return (TRUE);
}
