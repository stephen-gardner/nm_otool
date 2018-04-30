/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 08:47:17 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/30 12:11:38 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	*find_text_section(t_mchain *mchain)
{
	t_sec	*section;
	t_mlink	*mlink;

	mlink = mchain->start;
	while (mlink)
	{
		section = (t_sec *)mlink->ptr;
		if (!ft_strcmp(SEG_TEXT, section->segname)
			&& !ft_strcmp(SECT_TEXT, section->sectname))
			return ((void *)section);
		mlink = mlink->next;
	}
	return (NULL);
}

void		print_section(t_byte *start, t_byte *end, uint64_t addr)
{
	t_byte		*pos;
	int			n;

	n = 0;
	pos = start;
	while (pos < end)
	{
		if (!n)
			ft_printf("%016llx\t", addr + (pos - start));
		ft_printf("%.2x ", *pos);
		++pos;
		if (++n == 16)
		{
			n = 0;
			write(STDOUT_FILENO, "\n", 1);
		}
	}
	if (n)
		write(STDOUT_FILENO, "\n", 1);
}

t_bool		print_text_section(t_bin *bin, t_obj *obj)
{
	void		*section;
	t_mchain	*mchain;
	t_byte		*start;
	uint64_t	size;
	uint64_t	addr;

	if (!(mchain = ft_mcget("sections")))
		return (alloc_error());
	if (!find_lcmd(bin, obj, ((obj->is_64) ? LC_SEGMENT_64 : LC_SEGMENT)))
		return (TRUE);
	ft_printf("Contents of (__TEXT,__text) section\n");
	if (!(section = find_text_section(mchain)))
		return (TRUE);
	if (obj->is_64)
		start = obj->start + ((t_sec64 *)section)->offset;
	else
		start = obj->start + ((t_sec *)section)->offset;
	addr = (obj->is_64) ? ((t_sec64 *)section)->addr : ((t_sec *)section)->addr;
	size = (obj->is_64) ? ((t_sec64 *)section)->size : ((t_sec *)section)->size;
	if (start + size > bin->end)
		truncated_obj(bin, obj, TRUE);
	else
		print_section(start, start + size, addr);
	clean_mchain(mchain);
	return (TRUE);
}
