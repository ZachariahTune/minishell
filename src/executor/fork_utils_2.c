/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_is_dir(t_shell *shell, char *name)
{
	int	fd;

	fd = open(name, O_WRONLY);
	if (fd < 0 && errno == 21)
	{
		ft_putstr(name, 2);
		ft_putstr(" - is a directory\n", 2);
		ms_cmd_argv_free(shell->cmd);
		ms_shell_destroy(shell);
		exit(126);
	}
	else if (fd > 0)
		close(fd);
}

static char	*check_cmd(const char *cmd, size_t s_cmd, char *path, size_t s_path)
{
	char	*new;
	size_t	i;
	int		flg;

	flg = s_path != 0;
	i = -1;
	new = malloc(sizeof(char) * (s_cmd + s_path + 1 + flg));
	if (!new)
		return (NULL);
	while (++i < s_path)
		new[i] = path[i];
	if (flg)
		new[s_path] = '/';
	i = -1;
	while (++i < s_cmd)
		new[i + s_path + flg] = cmd[i];
	new[i + s_path + flg] = 0;
	if (!access(new, X_OK))
		return (new);
	free(new);
	return (NULL);
}

static char	parse_cmd(char **dst, const char *cmd, char *path)
{
	size_t		size_cmd;
	size_t		size_path;
	static char	*tmp;

	size_cmd = 0;
	if (!tmp)
		tmp = *dst;
	while (cmd[size_cmd])
		size_cmd++;
	*dst = check_cmd(cmd, size_cmd, path, 0);
	if (*dst)
		return (0);
	while (*path)
	{
		size_path = 0;
		while (path[size_path] && path[size_path] != ':')
			size_path++;
		*dst = check_cmd(cmd, size_cmd, path, size_path);
		if (*dst)
			return (0);
		path += size_path + (path[size_path] == ':');
	}
	*dst = tmp;
	return (1);
}

void	do_shell_command(t_shell *shell)
{
	size_t	i;
	char	*path;

	i = 0;
	while (shell->envp[i] && ft_strncmp("PATH=", shell->envp[i], 5) != 0)
		i++;
	if (!shell->envp[i] || ft_strchr(shell->cmd->argv[0], '/'))
		path = "";
	else
		path = (shell->envp[i]) + 5;
	if (parse_cmd(&((shell->cmd->argv)[0]), (shell->cmd->argv)[0], path))
	{
		ft_putstr("Command doesn't exist(", 2);
		ft_putstr((shell->cmd->argv)[0], 2);
		ft_putstr(").\n", 2);
		ms_cmd_argv_free(shell->cmd);
		ms_shell_destroy(shell);
		exit(127);
	}
	ft_is_dir(shell, shell->cmd->argv[0]);
	execve(shell->cmd->argv[0], shell->cmd->argv, shell->envp);
	ft_puterror(shell, 3, strerror(errno));
}

void	count_status(t_shell *shell, int status)
{
	int	exit_res;
	int	term;
	int	stop;

	exit_res = WIFEXITED(status);
	if (exit_res)
		shell->status = WEXITSTATUS(status);
	else
		term = WIFSIGNALED(status);
	if (!exit_res && term)
		shell->status = WTERMSIG(status) + 128;
	else
		stop = WIFSTOPPED(status);
	if (!exit_res && !term && stop)
		shell->status = WSTOPSIG(status);
}
