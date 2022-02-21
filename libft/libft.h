/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <dirent.h>
# include <errno.h>

# define GNL_BUFF_SIZE	32

typedef struct s_globlist	t_globlist;
typedef struct s_glob		t_glob;

typedef struct s_globlist
{
	char		*name;
	t_globlist	*next;
}	t_globlist;

typedef struct s_glob
{
	t_globlist	*folders;
	t_globlist	*scandirs;
	t_globlist	*t_folders;
	t_globlist	*t_scandirs;
	int			folders_count;
	int			scandir_ret;
	int			pattern_c;
	char		**pattern_a;
	int			pattern_i;
	int			simple_addr;
	int			malloc_error;
}	t_glob;

//MiniGlob
t_glob		*mg_glob_init(char *pattern);
t_globlist	*mg_glob_folders_add(t_glob *glob, char *name);
int			mg_value_with_star(char *str);
void		mg_before_scandirs(t_glob *glob);
int			mg_compare(char *s1, char *s2);
void		*mg_pathjoin(void *dst, const void *src, int if_slash);
int			mg_folders_list_to_array(t_glob *glob, char ***files);
int			mg_destroy_if_malloc_error(t_glob *glob, int ret);
void		mg_glob_folders_destroy(t_glob *glob);
void		mg_glob_scandirs_destroy(t_glob *glob);
void		mg_miniglob_destroy(t_glob *glob);

//LibFT
int			ft_isalpha(int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strlen(const char *str);
char		*ft_strcpy(char *dst, const char *src);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strdup(const char *s);
int			ft_split_count(char const *str, char c);
void		*ft_memset(void *str, int c, size_t len);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memjoin(void *dst, const void *src);
char		**ft_split(char const *s, char c);
int			ft_gnl(int fd, char **line);
int			ft_miniglob(char *pathpattern, char ***files);
void		ft_arrayfree(void ***array, size_t count);
void		ft_free(void **pointer);
char		*ft_itoa(int n);
int			ft_is_q(int c);
int			ft_is_1_q(int c);
int			ft_is_2_q(int c);
char		*ft_strchr(const char *s, int c);
int			ft_is_var_name(int c);
int			ft_is_low_dash(int c);
int			ft_isalnum(int c);
int			ft_is_begin_var(int c);

#endif
