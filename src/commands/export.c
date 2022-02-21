/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**copy_old_env(char **new_env, t_shell *shell, int j)
{
	if (new_env != NULL)
	{
		while (--j >= 0)
			new_env[j] = shell->envp[j];
		free(shell->envp);
		return (new_env);
	}
	return (shell->envp);
}

static void	make_new_env(char ***new_env, t_shell *shell, int j)
{
	int	flg;
	int	i;
	int	index;

	*new_env = copy_old_env(*new_env, shell, j);
	i = 0;
	while (++i < shell->cmd->argc)
	{
		flg = check_correct_var(shell, shell->cmd->argv[i], 0);
		if (flg)
			index = find_variable(*new_env, shell->cmd->argv[i]);
		if (flg == 2)
			j = app_end_var(new_env, shell->cmd->argv[i], j, index);
		else if (flg == 1)
			j = add_var(new_env, shell->cmd->argv[i], j, index);
	}
}

static void	sort_envp(char **env)
{
	char	*tmp;
	int		i;
	int		j;
	char	flg_sort;

	flg_sort = 0;
	j = -1;
	while (!flg_sort)
	{
		flg_sort = 1;
		i = 0;
		while (env[++i] && (i < j || j < 0))
		{
			if (ft_strcmp(env[i - 1], env[i]) > 0)
			{
				tmp = env[i - 1];
				env[i - 1] = env[i];
				env[i] = tmp;
				flg_sort = 0;
			}
		}
		j = i;
	}
}

static void	export_no_arg(t_shell *shell, char **env, int size)
{
	size_t	i;
	char	**new;

	i = -1;
	new = copy_env_for_export(env, size);
	if (!new)
	{
		shell->status = 2;
		return (ft_putstr("Error: not enough memory for export\n", 2));
	}
	sort_envp(new);
	while (new[++i] && new[i][0] != '_')
	{
		ft_print_next(new[i]);
		write(1, "\n", 1);
	}
	free(new);
}

void	ms_cmd_execute_export(t_shell *shell)
{
	int		j;
	int		cnt;
	char	**new_env;
	int		i;

	shell->status = 0;
	j = 0;
	if (shell->cmd->argc == 1)
		return (export_no_arg(shell, shell->envp, shell->env_size));
	while ((shell->envp)[j])
		j++;
	cnt = count_correct_var(shell, shell->cmd->argv + 1, shell->cmd->argc - 1);
	new_env = NULL;
	if (cnt > 0)
	{
		new_env = malloc(sizeof(char *) * (j + cnt + 1));
		if (!new_env)
			return (ft_putstr("Not enough memory for new envp\n", 2));
		shell->env_size = j + cnt + 1;
		i = -1;
		while (++i <= j + cnt)
			new_env[i] = NULL;
	}
	make_new_env(&new_env, shell, j);
	shell->envp = new_env;
}
