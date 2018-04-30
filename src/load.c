/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:03:14 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/29 17:46:56 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "ft_nm.h"

static t_bool	load_ar_obj(t_obj *obj, t_ar *ar_head, t_byte *pos, t_byte *end)
{
	if (ft_memcmp(ar_head->ar_name, AR_EFMT1, 3))
	{
		obj->namlen = 16;
		obj->name = ar_head->ar_name;
		obj->pos = pos + sizeof(t_ar);
	}
	else
	{
		obj->namlen = ft_atoi(ar_head->ar_name + 3);
		obj->name = (char *)pos + sizeof(t_ar);
		obj->pos = (t_byte *)obj->name + obj->namlen;
	}
	while (!obj->name[obj->namlen - 1] || obj->name[obj->namlen - 1] == ' ')
		--obj->namlen;
	obj->ar_size = ft_atoi(ar_head->ar_size);
	return (!ft_memcmp(ar_head->ar_fmag, ARFMAG, 2)
		&& pos + sizeof(t_ar) + obj->ar_size <= end);
}

t_obj			*get_next_obj(t_bin *bin)
{
	static t_obj	obj;
	t_ar			*ar_head;

	ft_memset(&obj, 0, sizeof(t_obj));
	if (!bin->is_ar)
	{
		obj.pos = bin->pos;
		return (&obj);
	}
	ar_head = (t_ar *)bin->pos;
	if (bin->pos + sizeof(t_ar) >= bin->end
		|| !load_ar_obj(&obj, ar_head, bin->pos, bin->end))
	{
		NM_PERR(bin->path, "truncated or malformed archive");
		return (NULL);
	}
	bin->pos += sizeof(t_ar);
	return (&obj);
}

t_bool			load_bin(t_bin *bin)
{
	int			fd;
	struct stat	bstats;
	size_t		size;

	if ((fd = open(bin->path, O_RDONLY)) < 0 || fstat(fd, &bstats) < 0)
	{
		NM_PERR(bin->path, ERRMSG);
		return (FALSE);
	}
	size = bstats.st_size;
	if (!(bin->data = ft_memalloc(size)))
		return (alloc_error());
	if (read(fd, bin->data, size) < (ssize_t)size)
	{
		NM_ERR;
		close(fd);
		free(bin->data);
		return (FALSE);
	}
	close(fd);
	if (!ft_memcmp(bin->data, ARMAG, SARMAG))
		bin->is_ar = TRUE;
	bin->pos = (bin->is_ar) ? bin->data + SARMAG : bin->data;
	bin->end = bin->data + size;
	return (TRUE);
}
