/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_cmd_execute_command(t_shell *shell)
{
	if (ft_strcmp(shell->cmd->argv[0], "cd") == 0)
		ms_cmd_execute_cd(shell);
	else if (ft_strcmp(shell->cmd->argv[0], "prompt") == 0)
		ms_cmd_execute_prompt(shell);
	else if (ft_strcmp(shell->cmd->argv[0], "pwd") == 0)
		ms_cmd_execute_pwd(shell);
	else if (ft_strcmp(shell->cmd->argv[0], "test") == 0)
		ms_cmd_execute_test(shell);
	else if (ft_strcmp(shell->cmd->argv[0], "exit") == 0)
		ms_cmd_execute_exit(shell);
	else if (ft_strcmp(shell->cmd->argv[0], "echo") == 0)
		ms_cmd_execute_echo(shell);
	else if (ft_strcmp(shell->cmd->argv[0], "env") == 0)
		ms_cmd_execute_env(shell);
	else if (shell->cmd->argv[0][0])
		do_shell_command(shell);
	ms_cmd_argv_free(shell->cmd);
	ms_shell_destroy(shell);
	exit(0);
}

static void	ms_execute_zombie_handler(int signum)
{
	int		stop;
	pid_t	pid;

	(void)signum;
	stop = 0;
	while (stop == 0)
	{
		pid = waitpid(-1, NULL, WNOHANG);
		if (pid > 0)
			printf("Process %d terminated\n", pid);
		if (pid <= 0)
			stop = 1;
	}
}

static void	ms_cmd_execute_after_fork(t_shell *shell, pid_t pid)
{
	struct sigaction	act;
	int					status;

	ms_signals_handler(shell, 2, pid);
	if (shell->cmd->async == 0)
	{
		waitpid(pid, &status, WUNTRACED);
		count_status(shell, status);
	}
	else if (shell->cmd->async == 1)
	{
		printf("Process %d started\n", pid);
		act.sa_flags = 0;
		act.sa_handler = ms_execute_zombie_handler;
		if (sigaction(SIGCHLD, &act, NULL) != 0)
		{
			ft_putstr(COLOR_RED, 2);
			perror("Error with sigaction()");
			ft_putstr(COLOR_RESET, 2);
		}
	}
}

static void	ms_write_heredoc_file(t_shell *shell)
{
	if (shell->cmd->redirect_dblin && MS_READLINE_REGIME == 1)
	{
		ms_write_heredoc_file_readline(shell);
		shell->cmd->redirect_in = shell->cmd->heredoc_file;
	}
	if (shell->cmd->redirect_dblin && MS_READLINE_REGIME == 2)
	{
		ms_write_heredoc_file_gnl(shell, 0);
		shell->cmd->redirect_in = shell->cmd->heredoc_file;
	}
}

void	ms_cmd_execute_fork(t_shell *shell)
{
	pid_t	pid;
	int		tempfd_stdout;

	ms_create_heredoc_file(shell);
	shell->status = 0;
	pid = fork();
	if (pid < 0)
		return (ms_cmd_execute_fork_error());
	if (pid == 0)
	{
		ms_signals_handler(shell, 1, 0);
		tempfd_stdout = dup(1);
		ms_cmd_execute_fd_null(shell);
		ms_write_heredoc_file(shell);
		ms_cmd_execute_fd_redirect_in(shell);
		ms_cmd_execute_fd_redirect_out(shell);
		if (shell->cmd->stdin_pipe)
			dup2(shell->cmd->pipe_read, 0);
		if (shell->cmd->stdout_pipe)
			dup2(shell->cmd->pipe_write, 1);
		if (ms_cmd_execute_command(shell) == 0)
			ms_cmd_execute_command_error(shell, tempfd_stdout);
	}
	ms_cmd_execute_after_fork(shell, pid);
}
