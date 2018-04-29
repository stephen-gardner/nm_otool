/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:02:43 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/28 21:39:44 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H
# include <ar.h>
# include <errno.h>
# include <mach-o/loader.h>
# include <stdio.h>
# include <unistd.h>
# include "libft.h"
# include "ft_printf.h"

typedef struct ar_hdr			t_ar;
typedef struct mach_header		t_mh;
typedef struct mach_header_64	t_mh64;
typedef struct load_command		t_lc;
typedef struct symtab_command	t_stabcmd;

# define PNAME			g_pname
# define ERRMSG			sys_errlist[errno]
# define NM_ERR			ft_dprintf(STDERR_FILENO, "%s: %s\n\n", PNAME, ERRMSG)
# define NM_PERR(x, y)	ft_dprintf(STDERR_FILENO, "%s: %s %s\n\n", PNAME, x, y)

typedef struct	s_obj
{
	t_byte		*start;
	t_byte		*pos;
	char		*name;
	uint32_t	namlen;
	uint32_t	ar_size;
	t_bool		is_64;
	t_bool		is_rev;
}				t_obj;

typedef struct	s_bin
{
	char		*path;
	t_byte		*data;
	t_byte		*pos;
	t_byte		*end;
	t_bool		is_ar;
}				t_bin;

/*
** load.c
*/

t_obj			*get_next_obj(t_bin *bin);
t_bool			load_bin(t_bin *bin, char *path);

/*
** mach.c
*/

t_bool			process_object(t_bin *bin, t_obj *obj);

/*
** util.c
*/

t_bool			alloc_error(const char *pre, const char *err);

extern const char	*g_pname;
#endif
