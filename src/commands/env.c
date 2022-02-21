/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_cmd_execute_env(t_shell *shell)
{
	size_t	i;
	int		j;
	char	**env;

	j = find_variable(shell->envp, "PATH");
	if (j < 0)
	{
		ft_putstr("Command doesn't exist (env).\n", 2);
		ms_cmd_argv_free(shell->cmd);
		ms_shell_destroy(shell);
		exit(127);
	}
	i = -1;
	env = shell->envp;
	while (env[++i])
	{
		ft_putstr(env[i], 1);
		write(1, "\n", 1);
	}
	ms_cmd_argv_free(shell->cmd);
	ms_shell_destroy(shell);
	exit(0);
}

char	**envp_cpy(char **env, t_shell *shell)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		ft_puterror(shell, 2, "(environment).\n");
	shell->env_size = i;
	new_env[i] = NULL;
	i = -1;
	while (new_env && env[++i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!(new_env[i]))
			ft_arrayfree((void ***)(&new_env), i);
	}
	return (new_env);
}

char	**copy_env_for_export(char **env, int size)
{
	char	**new;
	size_t	i;

	i = -1;
	new = malloc(sizeof(char *) * (size + 1));
	if (!new)
		return (NULL);
	while (env[++i])
		new[i] = env[i];
	new[i] = NULL;
	return (new);
}

void	ft_print_next(char *s)
{
	size_t	i;
	char	c;
	size_t	j;

	i = 0;
	j = 0;
	c = '"';
	write(1, "declare -x ", 11);
	while (s[i] && s[i] != '=')
		i++;
	write(1, s, i + 1);
	write(1, &c, 1);
	while (s[i + 1 + j])
		j++;
	write(1, (s + i + 1), j);
	write(1, &c, 1);
}
