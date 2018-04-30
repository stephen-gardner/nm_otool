/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 22:02:43 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/30 09:23:27 by sgardner         ###   ########.fr       */
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
# include "ft_memmgr.h"
# include "ft_printf.h"

typedef struct ar_hdr				t_ar;
typedef struct mach_header			t_mh;
typedef struct mach_header_64		t_mh64;
typedef struct load_command			t_lc;
typedef struct section				t_sec;
typedef struct section_64			t_sec64;
typedef struct segment_command		t_segcmd;
typedef struct segment_command_64	t_segcmd64;
typedef struct symtab_command		t_stabcmd;
typedef struct nlist				t_nlist;
typedef struct nlist_64				t_nlist64;

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
	uint32_t	ncmds;
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
t_bool			load_bin(t_bin *bin);

/*
** mach.c
*/

void			*find_lcmd(t_bin *bin, t_obj *obj, uint32_t cmd);
void			process_bin(t_bin *bin, t_bool print_text, t_bool multi);

/*
** segment.c
*/

char			find_section_symbol(uint8_t section);
t_bool			index_segment(t_bin *bin, t_obj *obj);

/*
** symtab.c
*/

t_bool			print_symtab(t_bin *bin, t_obj *obj);

/*
** text.c
*/

t_bool			print_text_section(t_bin *bin, t_obj *obj);

/*
** util.c
*/

t_bool			alloc_error(void);
void			clean_mchain(t_mchain *mchain);
int				nlist_cmp(t_obj *obj, t_mlink *m1, t_mlink *m2);
t_bool			truncated_obj(t_bin *bin, t_obj *obj, t_bool ret);

extern const char	*g_pname;
#endif
