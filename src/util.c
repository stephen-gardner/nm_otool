/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:07:11 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/27 12:57:04 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_nm.h"

t_bool	alloc_error(const char *pre, const char *err)
{
	if (pre)
	{
		write(STDOUT_FILENO, pre, LEN(pre));
		write(STDOUT_FILENO, ": ", 2);
	}
	write(STDOUT_FILENO, err, LEN(err));
	write(1, "\n", 1);
	return (FALSE);
}
