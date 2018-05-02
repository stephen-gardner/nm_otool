/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/27 22:04:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/02 16:09:50 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ranlib.h>
#include "ft_nm.h"

#define VBOOL(x)	(void *)((unsigned long)x)

void			*find_lcmd(t_bin *bin, t_obj *obj, uint32_t cmd)
{
	void		*res;
	t_lc		*lc;
	uint32_t	i;

	i = 0;
	res = NULL;
	while (i++ < obj->ncmds)
	{
		lc = (t_lc *)obj->pos;
		if (obj->pos + sizeof(t_lc) >= bin->end
			|| obj->pos + lc->cmdsize >= bin->end)
			return (VBOOL(truncated_obj(bin, obj, FALSE)));
		if (obj->is_rev)
			rev_lc(obj);
		if (lc->cmd == cmd)
			res = (void *)obj->pos;
		if (lc->cmd == LC_SEGMENT || lc->cmd == LC_SEGMENT_64)
		{
			if (!index_segment(obj))
				return (VBOOL(truncated_obj(bin, obj, FALSE)));
		}
		obj->pos += lc->cmdsize;
	}
	return (res);
}

static t_bool	process_object(t_bin *bin, t_obj *obj, t_bool print_text,
					t_bool multi)
{
	t_bool	res;

	obj->start = obj->pos;
	if (!valid_header(bin, obj))
	{
		if (bin->is_ar)
			bin->pos += obj->ar_size;
		ft_dprintf(STDERR_FILENO, "%s: %s %s\n", PNAME, bin->path,
			"The file was not recognized as a valid object file");
		return (bin->is_ar);
	}
	if (bin->is_ar)
		ft_printf("%s(%.*s):\n", bin->path, obj->namlen, obj->name);
	else if (print_text || multi)
		ft_printf("%s:\n", bin->path);
	if (obj->is_rev)
		rev_mh(obj);
	obj->ncmds = ((t_mh *)obj->pos)->ncmds;
	obj->pos += (obj->is_64) ? sizeof(t_mh64) : sizeof(t_mh);
	res = (print_text) ? print_text_section(bin, obj) : print_symtab(bin, obj);
	bin->pos = (bin->is_ar) ? bin->pos + obj->ar_size : bin->end;
	clean_mchains();
	return (res);
}

void			process_bin(t_bin *bin, t_bool print_text, t_bool multi)
{
	t_obj		*obj;
	t_bool		first;

	if (print_text && bin->is_ar)
		ft_printf("Archive : %s\n", bin->path);
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
		if (!print_text && (bin->is_ar || multi))
			write(STDOUT_FILENO, "\n", 1);
		if (!process_object(bin, obj, print_text, multi))
			break ;
		first = FALSE;
	}
}

static t_bool	process_universal(t_bin *bin, t_obj *obj, uint32_t magic)
{
	t_bool		is_rev;
	t_bool		is_64;

	if (obj->pos + sizeof(t_fh) >= bin->end)
		return (FALSE);
	is_rev = (magic == FAT_CIGAM || magic == FAT_CIGAM_64);
	is_64 = (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64);
	obj->pos += sizeof(t_fh);
	while (TRUE)
	{
		if (obj->pos + ((is_64) ? sizeof(t_fa64) : sizeof(t_fa)) >= bin->end)
			return (FALSE);
		if (is_rev)
			rev_fa(obj);
		if (((t_fa *)obj->pos)->cputype == CPU_TYPE_X86_64)
			break ;
		obj->pos += (is_64) ? sizeof(t_fa64) : sizeof(t_fa);
	}
	if (is_64)
		obj->pos = obj->start + ((t_fa64 *)obj->pos)->offset;
	else
		obj->pos = obj->start + ((t_fa *)obj->pos)->offset;
	obj->start = obj->pos;
	return (valid_header(bin, obj));
}

t_bool			valid_header(t_bin *bin, t_obj *obj)
{
	uint32_t	magic;

	if (obj->pos + sizeof(uint32_t) >= bin->end)
		return (FALSE);
	magic = *(uint32_t *)obj->pos;
	if (magic == FAT_MAGIC || magic == FAT_MAGIC_64
		|| magic == FAT_CIGAM || magic == FAT_CIGAM_64)
		return (process_universal(bin, obj, magic));
	else if (magic == MH_MAGIC)
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
