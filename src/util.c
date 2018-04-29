/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:07:11 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/29 01:37:12 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_nm.h"

t_bool	alloc_error(void)
{
	write(STDOUT_FILENO, PNAME, LEN(PNAME));
	write(STDOUT_FILENO, ": ", 2);
	write(STDOUT_FILENO, ERRMSG, LEN(ERRMSG));
	write(1, "\n", 1);
	return (FALSE);
}
