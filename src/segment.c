/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 07:02:59 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/02 01:20:12 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char			find_section_symbol(uint8_t section)
{
	t_mlink	*mlink;
	int		i;
	char	ret;

	i = 1;
	ret = 'U';
	mlink = (ft_mcget("sections"))->start;
	while (mlink)
	{
		if (section == i++)
		{
			ret = (char)mlink->size;
			break ;
		}
		mlink = mlink->next;
	}
	return (ret);
}

static char		get_symbol(char *name)
{
	if (!ft_strcmp(name, SECT_TEXT))
		return ('T');
	else if (!ft_strcmp(name, SECT_DATA))
		return ('D');
	else if (!ft_strcmp(name, SECT_BSS))
		return ('B');
	else
		return ('S');
}

static t_bool	index_sections(t_mchain *mchain, t_obj *obj, t_byte *pos,
					uint32_t nsects)
{
	int			size;
	uint32_t	i;
	char		sym;

	i = 0;
	size = (obj->is_64) ? sizeof(t_sec64) : sizeof(t_sec);
	while (i++ < nsects)
	{
		if (obj->is_rev)
			ft_revbytes(pos + 32, size - 32);
		sym = get_symbol(((t_sec *)pos)->sectname);
		if (!ft_mlappend(mchain, pos, (size_t)sym))
			return (FALSE);
		pos += size;
	}
	return (TRUE);
}

t_bool			index_segment(t_obj *obj)
{
	t_mchain	*mchain;
	t_byte		*pos;
	uint32_t	nsects;

	if (!(mchain = ft_mcget("sections")))
		return (FALSE);
	if (obj->is_64)
	{
		nsects = ((t_segcmd64 *)obj->pos)->nsects;
		pos = obj->pos + sizeof(t_segcmd64);
	}
	else
	{
		nsects = ((t_segcmd *)obj->pos)->nsects;
		pos = obj->pos + sizeof(t_segcmd);
	}
	return (index_sections(mchain, obj, pos, nsects));
}
