/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/29 00:20:45 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/30 11:57:23 by sgardner         ###   ########.fr       */
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
		label = (offset > 0) ? obj->start + symtab->stroff + offset : NULL;
		if (label >= bin->end)
			return (truncated_obj(bin, obj, TRUE));
		if (!(((t_nlist *)obj->pos)->n_type & N_STAB)
			&& !ft_mladd(mchain, obj->pos, (size_t)label))
			return (alloc_error());
		obj->pos += size;
	}
	return (TRUE);
}

static char			find_type(t_nlist64 *nlist, uint64_t address)
{
	uint8_t masked;
	char	res;

	res = nlist->n_type;
	masked = res & N_TYPE;
	if (masked == N_UNDF)
		res = (address) ? 'C' : 'U';
	else if (masked == N_ABS)
		res = 'A';
	else if (masked == N_INDR)
		res = 'I';
	else if (masked == N_SECT)
		res = find_section_symbol(nlist->n_sect);
	if (!(nlist->n_type & N_EXT))
		res = TO_LOWER(res);
	return (res);
}

static void			print_output(t_obj *obj, t_stabcmd *symtab,
						t_mchain *mchain)
{
	t_mlink		*mlink;
	t_nlist64	*nlist;
	uint64_t	address;
	char		type;

	UNUSED(symtab);
	mlink = mchain->start;
	while (mlink)
	{
		nlist = (t_nlist64 *)mlink->ptr;
		address = (obj->is_64) ? nlist->n_value : ((t_nlist *)nlist)->n_value;
		type = find_type(nlist, address);
		if (type != 'U')
			ft_printf("%016llx", address);
		else
			ft_printf("%16s", " ");
		ft_printf(" %c ", type);
		if (mlink->size)
			ft_printf("%s", (char *)mlink->size);
		write(STDOUT_FILENO, "\n", 1);
		mlink = ft_mlremove(mlink);
	}
}

static t_bool		sort_output(t_obj *obj, t_mchain *mchain)
{
	t_mlink		**mlink;
	t_mlink		*next;
	t_bool		moved;

	moved = FALSE;
	mlink = &mchain->start;
	while (*mlink && (next = (*mlink)->next))
	{
		if (nlist_cmp(obj, *mlink, next) > 0)
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

	if (!(symtab = find_lcmd(bin, obj, LC_SYMTAB)))
		return (TRUE);
	obj->pos = obj->start + symtab->symoff;
	if (!(mchain = ft_mcget("sym_table")))
		return (alloc_error());
	if (!build_output(bin, obj, symtab, mchain))
	{
		clean_mchain(mchain);
		return (FALSE);
	}
	while (sort_output(obj, mchain))
		;
	print_output(obj, symtab, mchain);
	clean_mchain(mchain);
	clean_mchain(ft_mcget("sections"));
	return (TRUE);
}
