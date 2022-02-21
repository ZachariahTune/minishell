/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Реакция на нажатие Ctrl-"C", Ctrl-"Z" и Ctrl-"\" до раздвоения
static void	ms_signal_handler(int sigtype)
{
	if (sigtype == SIGINT && MS_TEST_REGIME == 1)
		ft_putstr("\nVi nazhali \"Ctrl+C\"", 1);
	if (sigtype == SIGTSTP && MS_TEST_REGIME == 1)
		ft_putstr("\nVi nazhali \"Ctrl+Z\"", 1);
	if (sigtype == SIGQUIT && MS_TEST_REGIME == 1)
		ft_putstr("\nVi nazhali \"Ctrl+\\\"", 1);
	if (sigtype == SIGINT || sigtype == SIGTSTP || sigtype == SIGQUIT)
	{
		ft_putstr("\n", 1);
		if (MS_READLINE_REGIME == 1)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		if (MS_READLINE_REGIME == 2)
			ft_putstr(g_shell->prompt_line, 1);
	}
}

//Реакция на нажатие Ctrl-"Z" и Ctrl-"\" после раздвоения
static void	ms_signal_only_text(int sigtype)
{
	if (sigtype == SIGTSTP && MS_TEST_REGIME == 1)
		ft_putstr("\nVi nazhali \"Ctrl+Z\"", 1);
	if (sigtype == SIGQUIT)
	{
		ft_putstr("Exit", 1);
		if (MS_TEST_REGIME == 1)
			ft_putstr("\nVi nazhali \"Ctrl+\\\"", 1);
	}
	if (sigtype == SIGINT || sigtype == SIGTSTP || sigtype == SIGQUIT)
		ft_putstr("\n", 1);
}

//Реакция на нажатие Ctrl-"C" и Ctrl-"Z" в дочернем процессе
static void	ms_signal_ignore(int sigtype)
{
	(void)sigtype;
}

//Реакция на нажатие Ctrl-"C" в основном и Ctrl-"\" в дочернем процессе
static void	ms_signal_kill_or_exit(int sigtype)
{
	if (sigtype == SIGQUIT)
	{
		ms_cmd_argv_free(g_shell->cmd);
		ms_shell_destroy(g_shell);
		exit (131);
	}
	if (sigtype == SIGINT)
	{
		ft_putstr("Kill", 1);
		if (MS_TEST_REGIME == 1)
			ft_putstr("\nVi nazhali \"Ctrl+C\"", 1);
		ft_putstr("\n", 1);
		kill(g_shell->kill_pid, SIGKILL);
	}
}

//Перехват сигналов (type 0 - ожидание, 1 - дочерний, 2 - основной после fork)
void	ms_signals_handler(t_shell *shell, int type, pid_t pid)
{
	shell->kill_pid = pid;
	if (type == 0)
	{
		signal(SIGINT, ms_signal_handler);
		signal(SIGTSTP, ms_signal_handler);
		signal(SIGQUIT, ms_signal_handler);
	}
	if (type == 1)
	{
		signal(SIGINT, ms_signal_ignore);
		signal(SIGTSTP, ms_signal_ignore);
		signal(SIGQUIT, ms_signal_kill_or_exit);
	}
	if (type == 2)
	{
		signal(SIGINT, ms_signal_kill_or_exit);
		signal(SIGTSTP, ms_signal_only_text);
		signal(SIGQUIT, ms_signal_only_text);
	}
}
