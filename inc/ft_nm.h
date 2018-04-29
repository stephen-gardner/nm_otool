/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:02:43 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/29 04:46:43 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H
# include <ar.h>
# include <errno.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <stdio.h>
# include <unistd.h>
# include "libft.h"
# include "ft_printf.h"

typedef struct ar_hdr			t_ar;
typedef struct mach_header		t_mh;
typedef struct mach_header_64	t_mh64;
typedef struct load_command		t_lc;
typedef struct symtab_command	t_stabcmd;
typedef struct nlist			t_nlist;
typedef struct nlist_64			t_nlist64;

# define PNAME			g_pname
# define ERRMSG			sys_errlist[errno]
# define NM_ERR			ft_dprintf(STDERR_FILENO, "%s: %s\n", PNAME, ERRMSG)
# define NM_PERR(x, y)	ft_dprintf(STDERR_FILENO, "%s: %s %s\n", PNAME, x, y)

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

uint32_t		get_ncmds(t_obj *obj);
void			process_bin(t_bin *bin, t_bool print_text, t_bool multi);

/*
** symtab.c
*/

t_bool			print_symtab(t_bin *bin, t_obj *obj);

/*
** util.c
*/

t_bool			alloc_error(void);

extern const char	*g_pname;
#endif
