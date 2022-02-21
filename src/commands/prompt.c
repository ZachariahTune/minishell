/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_prompt_name(t_shell *shell, char *name)
{
	char	cwd[1000];
	int		name_len;
	int		prompt_line_len;

	name_len = ft_strlen(name);
	if (shell->prompt_name != NULL)
		ft_free((void **)&shell->prompt_name);
	if (shell->prompt_line != NULL)
		ft_free((void **)&shell->prompt_line);
	getcwd(cwd, 999);
	prompt_line_len = name_len + ft_strlen(cwd) + ft_strlen(COLOR_ORANGE)
		+ ft_strlen(COLOR_BLUE) + ft_strlen(COLOR_RESET) + 3;
	shell->prompt_name = (char *)malloc(name_len + 1);
	shell->prompt_line = (char *)malloc(prompt_line_len + 1);
	if (shell->prompt_name == NULL || shell->prompt_line == NULL)
		ft_puterror(shell, 2, "(privetstvie).\n");
	ft_strcpy(shell->prompt_name, name);
	ft_strcpy(shell->prompt_line, COLOR_ORANGE);
	ft_memjoin(shell->prompt_line, name);
	ft_memjoin(shell->prompt_line, ":");
	ft_memjoin(shell->prompt_line, COLOR_BLUE);
	ft_memjoin(shell->prompt_line, cwd);
	ft_memjoin(shell->prompt_line, "# ");
	ft_memjoin(shell->prompt_line, COLOR_RESET);
}

void	ms_prompt_cwd(t_shell *shell)
{
	char	cwd[1000];
	int		name_len;
	int		prompt_line_len;

	name_len = ft_strlen(shell->prompt_name);
	if (shell->prompt_line != NULL)
		ft_free((void **)&shell->prompt_line);
	getcwd(cwd, 999);
	prompt_line_len = name_len + ft_strlen(cwd) + ft_strlen(COLOR_ORANGE)
		+ ft_strlen(COLOR_BLUE) + ft_strlen(COLOR_RESET) + 3;
	shell->prompt_line = (char *)malloc(prompt_line_len + 1);
	if (shell->prompt_line == NULL)
		ft_puterror(shell, 2, "(privetstvie).\n");
	ft_strcpy(shell->prompt_line, COLOR_ORANGE);
	ft_memjoin(shell->prompt_line, shell->prompt_name);
	ft_memjoin(shell->prompt_line, ":");
	ft_memjoin(shell->prompt_line, COLOR_BLUE);
	ft_memjoin(shell->prompt_line, cwd);
	ft_memjoin(shell->prompt_line, "# ");
	ft_memjoin(shell->prompt_line, COLOR_RESET);
}

void	ms_cmd_execute_prompt(t_shell *shell)
{
	if (shell->cmd->argc == 1)
	{
		shell->status = 1;
		ft_putstr("prompt: neobhodimo ukazat text novogo privetstviya\n", 2);
	}
	else if (shell->cmd->argc > 2)
	{
		shell->status = 2;
		ft_putstr("Prompt: neobhodimo ukazat tolko odin parametr\n", 2);
	}
	else
	{
		shell->status = 0;
		ms_prompt_name(shell, shell->cmd->argv[1]);
	}
}
