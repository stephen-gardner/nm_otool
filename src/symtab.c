/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/29 00:20:45 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/02 16:03:12 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
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
			rev_nlist(obj);
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
	char		*out;
	char		type;

	UNUSED(symtab);
	mlink = mchain->start;
	while (mlink)
	{
		nlist = (t_nlist64 *)mlink->ptr;
		address = (obj->is_64) ? nlist->n_value : ((t_nlist *)nlist)->n_value;
		if (!(out = ft_memalloc(LEN((char *)mlink->size) + ADDR_LEN + 4)))
			return ((void)alloc_error());
		if ((type = find_type(nlist, address)) != 'U')
			ft_sprintf(out, "%0*llx", ADDR_LEN, address);
		else
			ft_sprintf(out, "%*s", ADDR_LEN, " ");
		ft_sprintf(out + ADDR_LEN, " %c ", type);
		if (mlink->size)
			ft_sprintf(out + ADDR_LEN + 3, "%s", (char *)mlink->size);
		ft_printf("%s\n", out);
		free(out);
		mlink = ft_mlremove(mlink);
	}
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
		return (FALSE);
	if (sort_output(obj, mchain))
		print_output(obj, symtab, mchain);
	return (TRUE);
}
