/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_cmd_execute_fork_error(void)
{
	ft_putstr(COLOR_RED, 2);
	perror("Error with fork()");
	ft_putstr(COLOR_RESET, 2);
}

void	ms_cmd_execute_fd_null(t_shell *shell)
{
	int	fd;

	if (shell->cmd->async == 1)
	{
		fd = open("/dev/null", O_RDWR);
		if (fd == -1)
		{
			ft_putstr(COLOR_RED, 2);
			perror("Error with /dev/null");
			ft_putstr(COLOR_RESET, 2);
			ms_cmd_argv_free(shell->cmd);
			ms_shell_destroy(shell);
			exit(1);
		}
		dup2(fd, 0);
	}
}

void	ms_cmd_execute_fd_redirect_in(t_shell *shell)
{
	int	fd;

	if (shell->cmd->redirect_in)
	{
		fd = open(shell->cmd->redirect_in, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr(COLOR_RED, 2);
			ft_putstr("Error with ", 2);
			perror(shell->cmd->redirect_in);
			ft_putstr(COLOR_RESET, 2);
			ms_cmd_argv_free(shell->cmd);
			ms_shell_destroy(shell);
			exit(1);
		}
		dup2(fd, 0);
	}
}

void	ms_cmd_execute_fd_redirect_out(t_shell *shell)
{
	int	fd;

	if (shell->cmd->redirect_out || shell->cmd->redirect_dblout)
	{
		if (shell->cmd->redirect_out)
			fd = open(shell->cmd->redirect_out,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (shell->cmd->redirect_dblout)
			fd = open(shell->cmd->redirect_dblout,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_putstr(COLOR_RED, 2);
			ft_putstr("Error with ", 2);
			if (shell->cmd->redirect_out)
				perror(shell->cmd->redirect_out);
			if (shell->cmd->redirect_dblout)
				perror(shell->cmd->redirect_dblout);
			ft_putstr(COLOR_RESET, 2);
			ms_cmd_argv_free(shell->cmd);
			ms_shell_destroy(shell);
			exit(1);
		}
		dup2(fd, 1);
	}
}

void	ms_cmd_execute_command_error(t_shell *shell, int tempfd_stdout)
{
	dup2(tempfd_stdout, 1);
	ft_putstr(COLOR_RED, 2);
	ft_putstr("Command not found: ", 2);
	ft_putstr(shell->cmd->argv[0], 2);
	ft_putstr("\n", 2);
	ft_putstr(COLOR_RESET, 2);
	ms_cmd_argv_free(shell->cmd);
	ms_shell_destroy(shell);
	exit(1);
}
