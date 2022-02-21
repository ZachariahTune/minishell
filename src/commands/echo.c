/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	skip_flg_n(t_cmd *cmd)
{
	size_t	i;
	size_t	res;

	res = 0;
	i = 0;
	while (++i < (size_t)cmd->argc && !ft_strcmp((cmd->argv)[i], "-n"))
		res++;
	return (res);
}

void	ms_cmd_execute_echo(t_shell *shell)
{
	size_t	flg_n;
	size_t	i;
	size_t	j;

	i = 0;
	if (shell->cmd->argc >= 2)
	{
		flg_n = skip_flg_n(shell->cmd);
		while (++i + flg_n < (size_t) shell->cmd->argc)
		{
			j = 0;
			while ((shell->cmd->argv)[i + flg_n][j])
				j++;
			write(1, shell->cmd->argv[i + flg_n], j);
			if (i + flg_n + 1 < (size_t) shell->cmd->argc)
				write(1, " ", 1);
		}
	}
	if (shell->cmd->argc < 2 || !flg_n)
		write(1, "\n", 1);
	ms_cmd_argv_free(shell->cmd);
	ms_shell_destroy(shell);
	exit(0);
}
