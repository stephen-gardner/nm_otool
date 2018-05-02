/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_headers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/02 11:18:49 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/02 15:56:06 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	rev_lc(t_obj *obj)
{
	t_lc	*lc;

	lc = (t_lc *)obj->pos;
	REV(lc->cmd, uint32_t);
	REV(lc->cmdsize, uint32_t);
}

void	rev_fa(t_obj *obj)
{
	t_fa	*fa;
	t_fa64	*fa64;

	if (obj->is_64)
	{
		fa64 = (t_fa64 *)obj->pos;
		REV(fa64->cputype, cpu_type_t);
		REV(fa64->cpusubtype, cpu_subtype_t);
		REV(fa64->offset, uint64_t);
		REV(fa64->size, uint64_t);
		REV(fa64->align, uint32_t);
		REV(fa64->reserved, uint32_t);
		return ;
	}
	fa = (t_fa *)obj->pos;
	REV(fa->cputype, cpu_type_t);
	REV(fa->cpusubtype, cpu_subtype_t);
	REV(fa->offset, uint32_t);
	REV(fa->size, uint32_t);
	REV(fa->align, uint32_t);
}

void	rev_mh(t_obj *obj)
{
	t_mh	*mh;
	t_mh64	*mh64;

	if (obj->is_64)
	{
		mh64 = (t_mh64 *)obj->pos;
		REV(mh64->cputype, cpu_type_t);
		REV(mh64->cpusubtype, cpu_subtype_t);
		REV(mh64->filetype, uint32_t);
		REV(mh64->ncmds, uint32_t);
		REV(mh64->sizeofcmds, uint32_t);
		REV(mh64->flags, uint32_t);
		REV(mh64->reserved, uint32_t);
		return ;
	}
	mh = (t_mh *)obj->pos;
	REV(mh->cputype, cpu_type_t);
	REV(mh->cpusubtype, cpu_subtype_t);
	REV(mh->filetype, uint32_t);
	REV(mh->ncmds, uint32_t);
	REV(mh->sizeofcmds, uint32_t);
	REV(mh->flags, uint32_t);
}

void	rev_segcmd(t_obj *obj)
{
	t_segcmd	*seg;
	t_segcmd64	*seg64;

	if (obj->is_64)
	{
		seg64 = (t_segcmd64 *)obj->pos;
		REV(seg64->vmaddr, uint64_t);
		REV(seg64->vmsize, uint64_t);
		REV(seg64->fileoff, uint64_t);
		REV(seg64->filesize, uint64_t);
		REV(seg64->maxprot, vm_prot_t);
		REV(seg64->initprot, vm_prot_t);
		REV(seg64->nsects, uint32_t);
		REV(seg64->flags, uint32_t);
		return ;
	}
	seg = (t_segcmd *)obj->pos;
	REV(seg->vmaddr, uint32_t);
	REV(seg->vmsize, uint32_t);
	REV(seg->fileoff, uint32_t);
	REV(seg->filesize, uint32_t);
	REV(seg->maxprot, vm_prot_t);
	REV(seg->initprot, vm_prot_t);
	REV(seg->nsects, uint32_t);
	REV(seg->flags, uint32_t);
}

void	rev_stabcmd(t_obj *obj)
{
	t_stabcmd	*stab;

	stab = (t_stabcmd *)obj->pos;
	REV(stab->symoff, uint32_t);
	REV(stab->nsyms, uint32_t);
	REV(stab->stroff, uint32_t);
	REV(stab->strsize, uint32_t);
}
