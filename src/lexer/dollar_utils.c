/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_var_len(char *s, int *i, t_shell *shell, int *flg)
{
	int		k;
	int		j;

	k = 0;
	*flg = 1;
	while (s[k] && ft_is_var_name(s[k]))
		k++;
	j = -1;
	while (++j < shell->env_size - 1)
	{
		if (!ft_strncmp(shell->envp[j], s, k) && shell->envp[j][k] == '=')
			break ;
	}
	*i = *i + k;
	if (shell->envp[j])
		return (ft_strlen(shell->envp[j] + k + 1));
	return (0);
}

static void	counts(int *res, int *i, char *s, int *flg)
{
	(*i)++;
	if (flg)
		*flg = 1;
	if (s)
		(*res) += ft_strlen(s);
	else if (res)
		(*res)++;
}

int	ft_str_real_len(char *s, t_shell *shell)
{
	int	res;
	int	i;
	int	flg;
	int	flg_q;

	res = 0;
	i = 0;
	flg = -1;
	flg_q = 0;
	while (s[i])
	{
		if (ft_is_1_q(s[i]))
			flg_q = !flg_q;
		if (!flg_q && s[i] == '$' && ft_is_begin_var(s[i + 1]))
			res += count_var_len(s + i + 1, &i, shell, &flg);
		else if (!flg_q && s[i] == '$' && s[i + 1] == '?')
			counts(&res, &i, shell->st, &flg);
		else if (!flg_q && s[i] == '$' && s[i + 1] > 47 && s[i + 1] < 58)
			counts(&res, &i, "", &flg);
		else if (!flg_q && s[i] == '$' && ft_is_q(s[i + 1]))
			counts(NULL, &i, NULL, &flg);
		else
			counts(&res, &i, NULL, NULL);
	}
	return (res * flg);
}
