/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_miniglob_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//Инициализация структуры Glob
t_glob	*mg_glob_init(char *pattern)
{
	t_glob	*glob;

	glob = (t_glob *)malloc(sizeof(t_glob));
	if (glob == NULL)
		return (NULL);
	glob->folders = NULL;
	glob->scandirs = NULL;
	glob->t_folders = NULL;
	glob->t_scandirs = NULL;
	glob->folders_count = 0;
	glob->scandir_ret = 0;
	glob->pattern_c = ft_split_count(pattern, '/');
	glob->pattern_a = ft_split(pattern, '/');
	glob->pattern_i = 0;
	glob->simple_addr = 0;
	glob->malloc_error = 0;
	return (glob);
}

//Создание элемента списка folders
t_globlist	*mg_glob_folders_add(t_glob *glob, char *name)
{
	t_globlist	*folders;

	(void)glob;
	folders = (t_globlist *)malloc(sizeof(t_globlist));
	if (folders == NULL)
	{
		return (NULL);
	}
	folders->name = (char *)malloc(ft_strlen(name) + 1);
	if (folders->name == NULL)
	{
		ft_free((void **)&folders);
		return (NULL);
	}
	ft_strcpy(folders->name, name);
	folders->next = NULL;
	return (folders);
}

//Сравнение двух строк на соответствие шаблону
int	mg_compare(char *s1, char *s2)
{
	if (ft_strcmp(s2, ".") == 0 || ft_strcmp(s2, "..") == 0)
		return (0);
	if ((!*s1 && !*s2))
		return (1);
	else if (*s1 == '?' && *s2)
		return (mg_compare(s1 + 1, s2 + 1));
	else if (*s1 == '*' && s1[1] == '*')
		return (mg_compare(s1 + 1, s2));
	else if (*s1 == '*' && *s2 == s1[1])
		return (mg_compare(s1 + 1, s2) || mg_compare(s1, s2 + 1));
	else if (*s1 == '*' && *s2 != s1[1] && *s2)
		return (mg_compare(s1, s2 + 1));
	else if (*s1 == '\\' && *s2 == s1[1]
		&& (s1[1] == '*' || s1[1] == '?' || s1[1] == '{' || s1[1] == '['))
		return (mg_compare(s1 + 2, s2 + 1));
	else if (*s1 == *s2)
		return (mg_compare(s1 + 1, s2 + 1));
	return (0);
}

//Пристыковка к строке символа "/" и значения "src"
void	*mg_pathjoin(void *dst, const void *src, int if_slash)
{
	if (if_slash == 1)
		dst = ft_memjoin(dst, "/");
	if (src != NULL)
		dst = ft_memjoin(dst, src);
	return (dst);
}

//Создание массива файлов на основе списка
int	mg_folders_list_to_array(t_glob *glob, char ***files)
{
	int	i;
	int	name_length;

	glob->t_folders = glob->folders;
	*files = malloc(sizeof(char *) * (glob->folders_count + 1));
	if (*files == NULL)
		return (-1);
	i = 0;
	while (glob->t_folders != NULL)
	{
		name_length = ft_strlen(glob->t_folders->name);
		(*files)[i] = malloc(sizeof(char) * (name_length + 1));
		if ((*files)[i] == NULL)
		{
			ft_arrayfree((void ***)files, i);
			return (-1);
		}
		ft_strcpy((*files)[i], glob->t_folders->name);
		glob->t_folders = glob->t_folders->next;
		i++;
	}
	(*files)[i] = NULL;
	return (i);
}
