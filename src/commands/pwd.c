/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_cmd_execute_pwd(t_shell *shell)
{
	char	cwd[1000];

	(void)shell;
	if (getcwd(cwd, 999) != NULL)
	{
		ft_putstr(cwd, 1);
		ft_putstr("\n", 1);
	}
	else
	{
		ft_putstr(COLOR_RED, 2);
		perror("Error with getcwd()");
		ft_putstr(COLOR_RESET, 2);
	}
}
