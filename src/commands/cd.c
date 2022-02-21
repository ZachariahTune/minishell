/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	too_many_arg(t_shell *shell)
{
	shell->status = 1;
	ft_putstr("cd: neobhodimo ukazat tolko odin parametr\n", 2);
}

static void	error_chdir(t_shell *shell)
{
	shell->status = 1;
	ft_putstr("Error with ", 2);
	ft_putstr(shell->cmd->argv[0], 2);
	ft_putstr(": ", 2);
	ft_putstr(shell->cmd->argv[1], 2);
	ft_putstr(" ", 2);
	ft_putstr(strerror(errno), 2);
	write(2, "\n", 1);
}

void	ms_cmd_execute_cd(t_shell *shell)
{
	if (shell->cmd->argc == 1)
	{
		shell->status = 0;
		chdir(getenv("HOME"));
		ms_prompt_cwd(shell);
	}
	else if (shell->cmd->argc > 2)
		too_many_arg(shell);
	else
	{
		if (chdir(shell->cmd->argv[1]) != 0)
			error_chdir(shell);
		else
		{
			shell->status = 0;
			ms_prompt_cwd(shell);
		}
	}
}
