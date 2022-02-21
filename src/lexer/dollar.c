/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_offset_dst(char *dst, int j, t_shell *shell, char *s)
{
	int	res;

	res = 0;
	if (!shell->envp[j])
		return (res);
	res = ft_strlen(s);
	ft_memcpy((void *)dst, (void *)s, res);
	return (res);
}

static size_t	copy_var(char *dst, char *src, t_shell *shell, size_t *i_dst)
{
	size_t	i;
	int		j;
	int		k;

	i = -1;
	j = -1;
	k = 0;
	if ((t_shell *)src == shell)
	{
		while (shell->st[++i])
			dst[i] = shell->st[i];
		*i_dst = *i_dst + i;
		return (2);
	}
	while (src[k] && ft_is_var_name(src[k]))
		k++;
	while (shell->envp[++j])
	{
		if (!ft_strncmp(shell->envp[j], src, k) && shell->envp[j][k] == '=')
			break ;
	}
	*i_dst += count_offset_dst(dst, j, shell, shell->envp[j] + k + 1);
	return (k);
}

static void	ms_replace_variable(char *dst, char *src, t_shell *shell)
{
	size_t	i_ds;
	size_t	i_sr;
	int		flg_q;

	i_sr = 0;
	i_ds = 0;
	flg_q = 0;
	while (src[i_sr])
	{
		if (ft_is_1_q(src[i_sr]))
			flg_q = !flg_q;
		if (!flg_q && src[i_sr] == '$' && ft_is_begin_var(src[i_sr + 1]))
			i_sr += copy_var(dst + i_ds, src + i_sr + 1, shell, &i_ds) + 1;
		else if (!flg_q && src[i_sr] == '$' && src[i_sr + 1] == '?')
			i_sr += copy_var(dst + i_ds, (char *)shell, shell, &i_ds);
		else if (!flg_q && src[i_sr] == '$' && src[i_sr + 1] > 47 \
		&& src[i_sr + 1] < 58)
			i_sr += 2;
		else if (!flg_q && src[i_sr] == '$' && ft_is_q(src[i_sr + 1]))
			i_sr++;
		else
			dst[i_ds++] = src[i_sr++];
	}
	dst[i_ds] = 0;
}

void	ms_lexerlist_replace_var(t_shell *shell)
{
	char	*tmp;
	int		flg_dollar;

	flg_dollar = ft_str_real_len(shell->input, shell);
	if (flg_dollar > 0)
	{
		tmp = shell->input;
		shell->input = malloc(sizeof(char) * (flg_dollar + 1));
		if (!shell->input)
		{
			shell->input = tmp;
			ft_putstr("Not enough memory\n", 2);
		}
		else
		{
			ms_replace_variable(shell->input, tmp, shell);
			free(tmp);
		}
	}
}
