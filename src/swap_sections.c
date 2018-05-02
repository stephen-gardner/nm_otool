/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_sections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/02 13:19:57 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/02 15:56:21 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		rev_nlist(t_obj *obj)
{
	t_nlist		*nlist;
	t_nlist64	*nlist64;

	if (obj->is_64)
	{
		nlist64 = (t_nlist64 *)obj->pos;
		REV(nlist64->n_un.n_strx, uint32_t);
		REV(nlist64->n_desc, uint16_t);
		REV(nlist64->n_value, uint64_t);
		return ;
	}
	nlist = (t_nlist *)obj->pos;
	REV(nlist->n_un.n_strx, uint32_t);
	REV(nlist->n_desc, int16_t);
	REV(nlist->n_value, uint32_t);
}

static void	rev_sec64(t_obj *obj)
{
	t_sec64 *s;

	s = (t_sec64 *)obj->pos;
	REV(s->addr, uint64_t);
	REV(s->size, uint64_t);
	REV(s->offset, uint32_t);
	REV(s->align, uint32_t);
	REV(s->reloff, uint32_t);
	REV(s->nreloc, uint32_t);
	REV(s->flags, uint32_t);
	REV(s->reserved1, uint32_t);
	REV(s->reserved2, uint32_t);
	REV(s->reserved3, uint32_t);
}

void		rev_sec(t_obj *obj)
{
	t_sec	*s;

	if (obj->is_64)
		return (rev_sec64(obj));
	s = (t_sec *)obj->pos;
	REV(s->addr, uint32_t);
	REV(s->size, uint32_t);
	REV(s->offset, uint32_t);
	REV(s->align, uint32_t);
	REV(s->reloff, uint32_t);
	REV(s->nreloc, uint32_t);
	REV(s->flags, uint32_t);
	REV(s->reserved1, uint32_t);
	REV(s->reserved2, uint32_t);
}
