/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 08:47:17 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/30 14:25:07 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	*find_text_section(t_mchain *mchain)
{
	t_sec	*sect;
	t_mlink	*mlink;

	mlink = mchain->start;
	while (mlink)
	{
		sect = (t_sec *)mlink->ptr;
		if (!ft_strcmp(SEG_TEXT, sect->segname)
			&& !ft_strcmp(SECT_TEXT, sect->sectname))
			return ((void *)sect);
		mlink = mlink->next;
	}
	return (NULL);
}

void		print_section(t_byte *start, t_byte *end, uint64_t addr)
{
	char		buf[68];
	t_byte		*pos;
	int			n;

	n = 0;
	pos = start;
	while (pos < end)
	{
		if (!n)
			ft_sprintf(buf, "%016llx\t", addr + (pos - start));
		ft_sprintf(buf + (n * 3) + 17, "%.2x ", *pos);
		++pos;
		if (++n == 16)
		{
			n = 0;
			ft_sprintf(buf + 65, "\n");
			write(STDOUT_FILENO, buf, 66);
		}
	}
	if (n)
	{
		ft_sprintf(buf + (n * 3) + 17, "\n");
		write(STDOUT_FILENO, buf, (n * 3) + 18);
	}
}

t_bool		print_text_section(t_bin *bin, t_obj *obj)
{
	void		*sect;
	t_mchain	*mchain;
	t_byte		*start;
	uint64_t	size;
	uint64_t	addr;

	if (!(mchain = ft_mcget("sections")))
		return (alloc_error());
	if (!find_lcmd(bin, obj, ((obj->is_64) ? LC_SEGMENT_64 : LC_SEGMENT)))
		return (TRUE);
	ft_printf("Contents of (%s,%s) section\n", SEG_TEXT, SECT_TEXT);
	if (!(sect = find_text_section(mchain)))
		return (TRUE);
	start = obj->start;
	start += (obj->is_64) ? ((t_sec64 *)sect)->offset : ((t_sec *)sect)->offset;
	addr = (obj->is_64) ? ((t_sec64 *)sect)->addr : ((t_sec *)sect)->addr;
	size = (obj->is_64) ? ((t_sec64 *)sect)->size : ((t_sec *)sect)->size;
	if (start + size > bin->end)
		return (truncated_obj(bin, obj, TRUE));
	print_section(start, start + size, addr);
	return (TRUE);
}
