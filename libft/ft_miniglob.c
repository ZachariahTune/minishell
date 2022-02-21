/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_miniglob.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//Наполнение списка найденных папок/файлов
static t_globlist	*mg_files_to_list(t_glob *glob,
	char files_names[3000][500], int files_count)
{
	int	i;

	i = 0;
	while (i < files_count)
	{
		if (glob->folders == NULL)
		{
			glob->folders = mg_glob_folders_add(glob, files_names[i]);
			glob->t_folders = glob->folders;
		}
		else
		{
			glob->t_folders->next = mg_glob_folders_add(glob, files_names[i]);
			glob->t_folders = glob->t_folders->next;
		}
		if (glob->t_folders == NULL)
		{
			mg_glob_folders_destroy(glob);
			glob->malloc_error = 1;
			return (NULL);
		}
		glob->folders_count++;
		i++;
	}
	return (glob->folders);
}

//Наполнение переменной files_names названиями найденных папок/файлов
static int	mg_filenames_build(t_glob *glob,
	char files_names[3000][500], char *filename, int i)
{
	files_names[i][0] = '\0';
	if ((glob->t_scandirs->name[0] != '.' && glob->t_scandirs->name[1] != '\0')
		|| glob->simple_addr == 0)
		ft_strcpy((char *)files_names[i], glob->t_scandirs->name);
	if ((glob->simple_addr == 1 && glob->t_scandirs->name[0] == '.'
			&& glob->t_scandirs->name[1] == '\0')
		|| (glob->t_scandirs->name[0] == '/'
			&& glob->t_scandirs->name[1] == '\0'))
		mg_pathjoin((char *)files_names[i], filename, 0);
	else
		mg_pathjoin((char *)files_names[i], filename, 1);
	i++;
	return (i);
}

//Сканирование папки и поиск вложенных папок/файлов по шаблону
static int	mg_scandirs(t_glob *glob, char *pattern, DIR *dir, int i)
{
	struct dirent	*file;
	char			files_names[3000][500];

	while (glob->t_scandirs != NULL)
	{
		dir = opendir(glob->t_scandirs->name);
		if (dir != NULL)
		{
			i = 0;
			while (1)
			{
				file = readdir(dir);
				if (file == NULL)
					break ;
				if (mg_compare(pattern, file->d_name) == 1)
					i = mg_filenames_build(glob, files_names, file->d_name, i);
			}
			closedir(dir);
			glob->folders = mg_files_to_list(glob, files_names, i);
			if ((i > 0 && glob->folders == NULL) || glob->malloc_error == 1)
				return (-1);
		}
		glob->t_scandirs = glob->t_scandirs->next;
	}
	return (glob->folders_count);
}

//Стартовая папка для первого сканирования (в зависимости от адреса шаблона)
static void	mg_get_first_folders(t_glob *glob, const char *pat)
{
	glob->pattern_i = 0;
	if (ft_strlen(pat) == 2 && pat[0] == '.' && pat[1] == '*')
		glob->folders = mg_glob_folders_add(glob, ".");
	if (ft_strlen(pat) == 3 && pat[0] == '.' && pat[1] == '.' && pat[2] == '*')
		glob->folders = mg_glob_folders_add(glob, "..");
	if (ft_strlen(pat) >= 1 && pat[0] == '/')
		glob->folders = mg_glob_folders_add(glob, "/");
	if (ft_strlen(pat) >= 1 && pat[0] != '.' && pat[0] != '/')
	{
		glob->folders = mg_glob_folders_add(glob, ".");
		glob->simple_addr = 1;
	}
	if ((ft_strlen(pat) >= 3 && pat[0] == '.' && pat[1] == '.' && pat[2] == '/')
		|| (ft_strlen(pat) >= 2 && pat[0] == '.' && pat[1] == '/'))
	{
		glob->folders = mg_glob_folders_add(glob, glob->pattern_a[0]);
		glob->pattern_i = 1;
	}
	if (glob->folders == NULL)
		glob->malloc_error = 1;
}

//Основная функция
int	ft_miniglob(char *pattern, char ***files)
{
	t_glob	*glob;
	int		files_count;
	char	*t_pattern;

	*files = NULL;
	if (ft_strlen(pattern) == 0 || mg_value_with_star(pattern) == 0)
		return (0);
	glob = mg_glob_init(pattern);
	if (glob == NULL || glob->pattern_a == NULL)
		return (-1);
	mg_get_first_folders(glob, pattern);
	if (glob->folders == NULL)
		return (mg_destroy_if_malloc_error(glob, -1));
	while (glob->pattern_i < glob->pattern_c)
	{
		mg_before_scandirs(glob);
		t_pattern = glob->pattern_a[glob->pattern_i];
		glob->scandir_ret = mg_scandirs(glob, t_pattern, NULL, 0);
		if (glob->scandir_ret < 0)
			return (mg_destroy_if_malloc_error(glob, glob->scandir_ret));
		glob->pattern_i++;
	}
	files_count = mg_folders_list_to_array(glob, files);
	mg_miniglob_destroy(glob);
	return (files_count);
}
