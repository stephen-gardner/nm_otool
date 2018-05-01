/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 14:39:08 by sgardner          #+#    #+#             */
/*   Updated: 2018/05/01 02:20:27 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_nm.h"

static int	nlist_cmp(t_obj *obj, t_mlink *m1, t_mlink *m2)
{
	uint64_t	a1;
	uint64_t	a2;
	int			res;

	if ((res = ft_strcmp((char *)m1->size, (char *)m2->size)))
		return (res);
	if (obj->is_64)
	{
		a1 = ((t_nlist64 *)m1->ptr)->n_value;
		a2 = ((t_nlist64 *)m2->ptr)->n_value;
	}
	else
	{
		a1 = ((t_nlist *)m1->ptr)->n_value;
		a2 = ((t_nlist *)m2->ptr)->n_value;
	}
	return (a1 > a2);
}

static void	max_heapify(t_obj *obj, t_mlink **arr, int node, int size)
{
	int		high;
	int		left;
	int		right;
	t_mlink	*tmp;

	while (TRUE)
	{
		high = node;
		left = (2 * node) + 1;
		right = (2 * node) + 2;
		if (left < size && nlist_cmp(obj, arr[left], arr[high]) > 0)
			high = left;
		if (right < size && nlist_cmp(obj, arr[right], arr[high]) > 0)
			high = right;
		if (high != node)
		{
			tmp = arr[node];
			arr[node] = arr[high];
			arr[high] = tmp;
			node = high;
		}
		else
			break ;
	}
}

static void	heap_sort(t_obj *obj, t_mlink **arr, int size)
{
	t_mlink	*tmp;
	int		i;

	i = size / 2;
	while (i > 0)
		max_heapify(obj, arr, --i, size);
	i = size - 1;
	while (i > 0)
	{
		tmp = arr[0];
		arr[0] = arr[i];
		arr[i] = tmp;
		max_heapify(obj, arr, 0, i--);
	}
}

t_bool		sort_output(t_obj *obj, t_mchain *mchain)
{
	t_mlink	**arr;
	t_mlink	*mlink;
	int		i;

	i = 0;
	if (!(arr = ft_memalloc(sizeof(t_mlink *) * (mchain->link_count + 1))))
		return (alloc_error());
	mlink = mchain->start;
	while (i < mchain->link_count)
	{
		arr[i++] = mlink;
		mlink = mlink->next;
	}
	heap_sort(obj, arr, i);
	mchain->start = arr[0];
	mchain->end = arr[mchain->link_count - 1];
	i = 0;
	mlink = mchain->start;
	while (i < mchain->link_count)
	{
		mlink->next = arr[++i];
		mlink = mlink->next;
	}
	free(arr);
	return (TRUE);
}
