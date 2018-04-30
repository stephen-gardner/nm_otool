/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/29 00:20:45 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/29 22:24:03 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static t_bool		build_output(t_bin *bin, t_obj *obj, t_stabcmd *symtab,
						t_mchain *mchain)
{
	t_byte		*label;
	int			size;
	uint32_t	offset;
	uint32_t	i;

	if (!symtab->nsyms)
		return (TRUE);
	i = 0;
	while (i++ < symtab->nsyms)
	{
		size = (obj->is_64) ? sizeof(t_nlist64) : sizeof(t_nlist);
		if (obj->pos + size > bin->end)
			return (truncated_obj(bin, obj, TRUE));
		if (obj->is_rev)
			ft_revbytes(obj->pos, size);
		offset = ((t_nlist *)obj->pos)->n_un.n_strx;
		label = (offset) ? obj->start + symtab->stroff + offset : NULL;
		if (label >= bin->end)
			return (truncated_obj(bin, obj, TRUE));
		if (!ft_mladd(mchain, obj->pos, (size_t)label))
			return (alloc_error());
		obj->pos += size;
	}
	return (TRUE);
}

static void			print_output(t_obj *obj, t_stabcmd *symtab,
						t_mchain *mchain)
{
	t_mlink		*mlink;
	t_nlist		*nlist;
	t_nlist64	*nlist64;

	UNUSED(symtab);
	mlink = mchain->start;
	while (mlink)
	{
		if (obj->is_64)
		{
			nlist64 = (t_nlist64 *)mlink->ptr;
			if (nlist64->n_value)
				ft_printf("%.16x ", nlist64->n_value);
			else
				ft_printf("%16s ", " ");
			ft_printf("%2d ", nlist64->n_type);
			ft_printf("%s\n", (char *)mlink->size);
		}
		else
		{
			UNUSED(nlist);
		}
		mlink = ft_mlremove(mlink);
	}
}

static t_bool		sort_output(t_mchain *mchain)
{
	t_mlink		**mlink;
	t_mlink		*next;
	t_bool		moved;

	moved = FALSE;
	mlink = &mchain->start;
	while (*mlink && (next = (*mlink)->next))
	{
		if (ft_strcmp((char *)(*mlink)->size, (char *)next->size) > 0)
		{
			(*mlink)->next = next->next;
			next->next = *mlink;
			*mlink = next;
			moved = TRUE;
		}
		mlink = &(*mlink)->next;
	}
	return (moved);
}

t_bool				print_symtab(t_bin *bin, t_obj *obj)
{
	t_mchain	*mchain;
	t_stabcmd	*symtab;

	if (!(symtab = find_lcmd(bin, obj, LC_SYMTAB, sizeof(t_stabcmd))))
		return (TRUE);
	obj->pos = obj->start + symtab->symoff;
	if (!(mchain = ft_mcget("ft_nm")))
		return (alloc_error());
	if (!build_output(bin, obj, symtab, mchain))
	{
		clean_mchain(mchain);
		return (FALSE);
	}
	while (sort_output(mchain))
		;
	print_output(obj, symtab, mchain);
	clean_mchain(mchain);
	return (TRUE);
}
