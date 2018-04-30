/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/27 22:04:24 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/30 08:13:58 by sgardner         ###   ########.fr       */
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
			ft_revbytes(obj->pos, lc->cmdsize);
		if (lc->cmd == cmd)
			res = (void *)obj->pos;
		else if ((lc->cmd == LC_SEGMENT || lc->cmd == LC_SEGMENT_64)
			&& !index_segment(bin, obj))
			return (VBOOL(truncated_obj(bin, obj, FALSE)));
		obj->pos += lc->cmdsize;
	}
	return (res);
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
		if (bin->is_ar)
		{
			bin->pos += obj->ar_size;
			return (TRUE);
		}
		NM_PERR(bin->path,
			"The file was not recognized as a valid object file");
		return (FALSE);
	}
	if (bin->is_ar)
		ft_printf("%s(%.*s):\n", bin->path, obj->namlen, obj->name);
	else if (print_text || multi)
		ft_printf("%s:\n", bin->path);
	if (obj->is_rev)
		ft_revbytes(obj->pos, (obj->is_64) ? sizeof(t_mh64) : sizeof(t_mh));
	obj->ncmds = ((t_mh *)obj->pos)->ncmds;
	obj->pos += (obj->is_64) ? sizeof(t_mh64) : sizeof(t_mh);
	res = (print_text) ? TRUE : print_symtab(bin, obj);
	bin->pos = (bin->is_ar) ? bin->pos + obj->ar_size : bin->end;
	return (res);
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
		if (!print_text && (bin->is_ar || multi))
			write(STDOUT_FILENO, "\n", 1);
		if (!process_object(bin, obj, print_text, multi))
			break ;
		first = FALSE;
	}
}
