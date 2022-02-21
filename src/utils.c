/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Запуск Shell (проверка количества параметров)
void	ms_shell_starterror(int argc, char **argv)
{
	(void)argv;
	if (argc != 1)
	{
		ft_puterror(NULL, 1, "");
	}
}

//Инициализация структуры Shell
t_shell	*ms_shell_init(void)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (shell == NULL)
		ft_puterror(NULL, 2, "(struktura Shell).\n");
	shell->prompt_name = NULL;
	shell->prompt_line = NULL;
	shell->input = NULL;
	shell->inputlen = 0;
	shell->input_i = 0;
	shell->input_c = 0;
	shell->lexerstate = LEXER_STATE_DEFAULT;
	shell->lexercount = 0;
	shell->lexerlist = NULL;
	shell->templexer = NULL;
	shell->lexval_i = 0;
	shell->lexertree = NULL;
	shell->trlist = NULL;
	shell->temptrlist = NULL;
	shell->cmd = NULL;
	shell->env_size = 0;
	ms_prompt_name(shell, MSH_DEFNAME);
	g_shell = shell;
	return (shell);
}

// Некоторые начальные параметры и удаление heredoc 
static void	ms_before_readline(t_shell *shell)
{
	ms_signals_handler(shell, 0, 0);
	shell->output_error = 0;
	shell->lexercount = 0;
	if (shell->cmd != NULL && shell->cmd->heredoc_file != NULL)
	{
		shell->cmd->heredoc_fd = 0;
		unlink(shell->cmd->heredoc_file);
		ft_free((void **)&shell->cmd->heredoc_file);
	}
}

// Считывание командной строки и запуск лексера
void	ms_readline_and_lexerlist(t_shell *shell)
{
	ms_before_readline(shell);
	if (MS_READLINE_REGIME == 1)
	{
		shell->input = readline(shell->prompt_line);
		if (shell->input == NULL)
			ft_putexit(shell);
	}
	if (MS_READLINE_REGIME == 2)
	{
		ft_putstr(shell->prompt_line, 1);
		while (shell->input == NULL)
			if (ft_gnl(0, &shell->input) == 0)
				ft_putexit(shell);
	}
	if (shell->input[0])
	{
		if (MS_READLINE_REGIME == 1)
			add_history(shell->input);
		ms_lexerlist_replace_var(shell);
		shell->inputlen = ft_strlen(shell->input);
		ms_lexerlist_build(shell);
	}
	if (shell->lexercount > 0)
		ms_lexerlist_corrector(shell);
	ft_free((void **)&shell->input);
}

// Удаление структуры Shell и всех ее компонентов
void	ms_shell_destroy(t_shell *shell)
{
	if (shell != NULL)
	{
		if (MS_READLINE_REGIME == 1)
			rl_clear_history();
		if (shell->input != NULL)
			ft_free((void **)&shell->input);
		if (shell->prompt_name != NULL)
			ft_free((void **)&shell->prompt_name);
		if (shell->prompt_line != NULL)
			ft_free((void **)&shell->prompt_line);
		if (shell->lexerlist != NULL)
			ms_lexerlist_destroy(shell);
		if (shell->lexertree != NULL)
			ms_parse_tree_destroy(shell);
		if (shell->trlist != NULL)
			ms_trlist_destroy(shell);
		if (shell->cmd != NULL)
			ft_free((void **)&shell->cmd);
		ft_arrayfree((void ***)&shell->envp, shell->env_size);
		ft_free((void **)&shell->st);
		ft_free((void **)&shell);
		shell = NULL;
	}
}
