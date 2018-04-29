/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_revbytes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/26 09:24:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/26 09:27:10 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_byte	*ft_revbytes(t_byte *data, size_t n)
{
	size_t	i;
	size_t	half;
	t_byte	swap;

	i = 0;
	half = n / 2;
	while (i < half)
	{
		swap = data[i];
		data[i] = data[n - i - 1];
		data[n - i - 1] = swap;
		++i;
	}
	return (data);
}
