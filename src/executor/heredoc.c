/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_heredoc_randchar(void)
{
	char	buff[1];
	int		nbr;
	int		fd;

	fd = open("/dev/random", O_RDONLY);
	if (fd < -1)
		return (-1);
	read(fd, buff, 1);
	nbr = (int)buff[0];
	if (nbr < 0)
		nbr = nbr * (-1);
	close(fd);
	return ('a' + nbr % 26);
}

static char	*ms_heredoc_randname(t_shell *shell)
{
	char	*randname;
	int		i;

	i = 0;
	randname = malloc(sizeof(char) * 11);
	if (randname == NULL)
	{
		ms_cmd_argv_free(shell->cmd);
		ft_puterror(shell, 2, "(imya heredoc).\n");
	}
	randname[10] = '\0';
	while (i < 10)
	{
		randname[i] = (char)ms_heredoc_randchar();
		i++;
	}
	return (randname);
}

void	ms_create_heredoc_file(t_shell *shell)
{
	int		fd;
	char	*filename;

	if (shell->cmd->redirect_dblin)
	{
		filename = NULL;
		fd = -1;
		while (fd == -1)
		{
			if (filename)
				ft_free((void **)&filename);
			filename = ms_heredoc_randname(shell);
			fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0644);
		}
		if (shell->cmd->heredoc_file != NULL)
		{
			unlink(shell->cmd->heredoc_file);
			ft_free((void **)&shell->cmd->heredoc_file);
		}
		shell->cmd->heredoc_fd = fd;
		shell->cmd->heredoc_file = filename;
	}
}

void	ms_write_heredoc_file_readline(t_shell *shell)
{
	char	*str;

	str = NULL;
	while (1)
	{
		str = readline("> ");
		if (str == NULL)
		{
			printf("!!! heredoc razdelen znakom konca faila vmesto «%s»\n",
				shell->cmd->redirect_dblin);
			break ;
		}
		if (ft_strcmp(shell->cmd->redirect_dblin, str) == 0)
			break ;
		write(shell->cmd->heredoc_fd, str, ft_strlen(str));
		write(shell->cmd->heredoc_fd, "\n", 1);
		ft_free((void **)&str);
	}
	ft_free((void **)&str);
}

void	ms_write_heredoc_file_gnl(t_shell *shell, int stop)
{
	char	*str;

	str = NULL;
	while (1)
	{
		ft_putstr("> ", 1);
		while (str == NULL)
		{
			if (ft_gnl(0, &str) == 0)
			{
				printf("!!! Heredoc razdelen znakom konca faila vmesto «%s»\n",
					shell->cmd->redirect_dblin);
				stop = 1;
				break ;
			}
		}
		if (stop == 1)
			break ;
		if (ft_strcmp(shell->cmd->redirect_dblin, str) == 0)
			break ;
		write(shell->cmd->heredoc_fd, str, ft_strlen(str));
		write(shell->cmd->heredoc_fd, "\n", 1);
		ft_free((void **)&str);
	}
	ft_free((void **)&str);
}
