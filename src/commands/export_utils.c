/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_correct_var(t_shell *shell, char *s, char flg)
{
	size_t	i;
	char	flg_correct;

	i = 0;
	flg_correct = 1;
	if (!ft_isalpha(s[i++]))
		flg_correct = 0;
	while (flg_correct && (ft_isalpha(s[i]) || (s[i] >= '0' && s[i] <= '9')))
		i++;
	if (flg_correct && s[i] == '+' && s[i + 1] == '=')
		return (2);
	if (flg_correct && s[i] == '=')
		return (1);
	if ((s[i] || !flg_correct) && flg)
	{
		ft_putstr("variable incorrect - ", 2);
		ft_putstr(s, 2);
		write(2, "\n", 1);
		shell->status = 1;
	}
	return (0);
}

static int	no_var(char **env, char *s)
{
	size_t	i;
	size_t	size_s;

	i = -1;
	size_s = 0;
	while (s[size_s] != '+' && s[size_s] != '=')
		size_s++;
	while (env[++i])
	{
		if (ft_strncmp(env[i], s, size_s) == 0)
			return (0);
	}
	return (1);
}

int	count_correct_var(t_shell *shell, char **argv, int argc)
{
	int	res;
	int	i;

	res = 0;
	i = -1;
	while (++i < argc)
	{
		if (check_correct_var(shell, argv[i], 1) \
		&& no_var(shell->envp, argv[i]))
			res++;
	}
	return (res);
}

int	add_var(char ***new_env, char *s, int j, int index)
{
	char	*tmp;

	if (index >= 0)
	{
		tmp = (*new_env)[index];
		(*new_env)[index] = ft_strdup(s);
		if (!(*new_env)[index])
		{
			ft_putstr("not enough memory for replace variable\n", 2);
			(*new_env)[index] = tmp;
		}
		else
			free(tmp);
		return (j);
	}
	(*new_env)[j] = ft_strdup(s);
	if (!(*new_env)[j])
	{
		ft_putstr("not enough memory for new variable\n", 2);
		return (j);
	}
	delete_if_need((*new_env)[j]);
	return (++j);
}

int	app_end_var(char ***new_env, char *s, int j, int index)
{
	char	*tmp;
	size_t	size;

	if (index < 0)
		return (add_var(new_env, s, j, index));
	tmp = (*new_env)[index];
	size = 0;
	while (s[size] != '+')
		size++;
	(*new_env)[index] = malloc(ft_strlen(tmp) + \
	ft_strlen(s + size + 1) * sizeof(char));
	if (!(*new_env)[index])
	{
		ft_putstr("not enough memory for append variable\n", 2);
		(*new_env)[index] = tmp;
		return (j);
	}
	size = -1;
	while (tmp[++size])
		(*new_env)[index][size] = tmp[size];
	(*new_env)[index][size] = '\0';
	(*new_env)[index] = ft_memjoin((void *)((*new_env)[index]), \
	(void *)(s + size - 1));
	free(tmp);
	return (j);
}
