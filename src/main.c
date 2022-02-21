/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*g_shell;

static void	ms_shell_status_string(t_shell *shell)
{
	char	*tmp;

	if (!shell->st)
	{
		shell->st = ft_itoa(shell->status);
		if (!shell->st)
			return (ft_putstr("Not enough memory for status\n", 2));
	}
	tmp = shell->st;
	shell->st = ft_itoa(shell->status);
	if (!shell->st)
	{
		shell->st = tmp;
		return (ft_putstr("Not enough memory for status\n", 2));
	}
	free(tmp);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	ms_shell_starterror(argc, argv);
	shell = ms_shell_init();
	shell->envp = envp_cpy(envp, shell);
	shell->status = 0;
	shell->st = NULL;
	while (1)
	{
		ms_shell_status_string(shell);
		ms_readline_and_lexerlist(shell);
		if (shell->lexercount > 0 && MS_TEST_REGIME == 1)
			ms_lexerlist_print(shell);
		if (ms_lexerlist_parse(shell) == 0)
		{
			if (MS_TEST_REGIME == 1)
				ms_lexertree_print(shell);
			ms_tree_execute(shell);
		}
		ms_lexerlist_destroy(shell);
		ms_parse_tree_destroy(shell);
		ms_trlist_destroy(shell);
	}
}
