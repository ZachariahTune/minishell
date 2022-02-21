/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_miniglob_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//Все работает только тогда, когда у аргумента есть '*'
int	mg_value_with_star(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

//Подготовительные действия перед сканированием папки
void	mg_before_scandirs(t_glob *glob)
{
	mg_glob_scandirs_destroy(glob);
	glob->scandirs = glob->folders;
	glob->folders = NULL;
	glob->t_scandirs = glob->scandirs;
	glob->folders_count = 0;
}

//Запускается, если произошла ошибка
int	mg_destroy_if_malloc_error(t_glob *glob, int ret)
{
	if (glob->malloc_error == 1)
		mg_miniglob_destroy(glob);
	return (ret);
}
