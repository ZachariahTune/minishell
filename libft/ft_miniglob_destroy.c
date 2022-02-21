/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_miniglob_destroy.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	mg_glob_folders_free(t_globlist *folders)
{
	if (folders->name != NULL)
		ft_free((void **)&folders->name);
	if (folders->next != NULL)
		mg_glob_folders_free(folders->next);
	ft_free((void **)&folders);
}

void	mg_glob_folders_destroy(t_glob *glob)
{
	if (glob->folders != NULL)
	{
		mg_glob_folders_free(glob->folders);
		glob->folders = NULL;
	}
}

static void	mg_glob_scandirs_free(t_globlist *scandirs)
{
	if (scandirs->name != NULL)
		ft_free((void **)&scandirs->name);
	if (scandirs->next != NULL)
		mg_glob_scandirs_free(scandirs->next);
	ft_free((void **)&scandirs);
}

void	mg_glob_scandirs_destroy(t_glob *glob)
{
	if (glob->scandirs != NULL)
	{
		mg_glob_scandirs_free(glob->scandirs);
		glob->scandirs = NULL;
	}
}

void	mg_miniglob_destroy(t_glob *glob)
{
	ft_arrayfree((void ***)&glob->pattern_a, glob->pattern_c);
	mg_glob_folders_destroy(glob);
	mg_glob_scandirs_destroy(glob);
	ft_free((void **)&glob);
}
