/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _corrector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_lexerlist_corrector_arrows(t_shell *shell)
{
	t_lexer	*templexer;
	char	*value;

	templexer = shell->lexerlist;
	while (templexer->next)
	{
		value = templexer->value;
		if (ft_strlen(value) == 2 && value[0] == '>' && value[1] == '>')
		{
			templexer->type = LEXER_TYPE_DLBOUT;
			shell->lexercount--;
		}
		if (ft_strlen(value) == 2 && value[0] == '<' && value[1] == '<')
		{
			templexer->type = LEXER_TYPE_DLBIN;
			shell->lexercount--;
		}
		if ((ft_strlen(value) > 2 && value[0] == '>' && value[1] == '>')
			|| (ft_strlen(value) > 2 && value[0] == '<' && value[1] == '<'))
		{
			templexer->type = LEXER_TYPE_ERROR;
			shell->lexercount--;
		}
		templexer = templexer->next;
	}
}

static void	ms_lexerlist_corrector_amps_pipes(t_shell *shell)
{
	t_lexer	*templexer;
	char	*value;

	templexer = shell->lexerlist;
	while (templexer->next)
	{
		value = templexer->value;
		if (ft_strlen(value) == 2 && value[0] == '&' && value[1] == '&')
		{
			templexer->type = LEXER_TYPE_DLBAMP;
			shell->lexercount--;
		}
		if (ft_strlen(value) == 2 && value[0] == '|' && value[1] == '|')
		{
			templexer->type = LEXER_TYPE_DLBPIPE;
			shell->lexercount--;
		}
		if ((ft_strlen(value) > 2 && value[0] == '&' && value[1] == '&')
			|| (ft_strlen(value) > 2 && value[0] == '|' && value[1] == '|'))
		{
			templexer->type = LEXER_TYPE_ERROR;
			shell->lexercount--;
		}
		templexer = templexer->next;
	}
}

void	ms_lexerlist_corrector(t_shell *shell)
{
	char	*tmp;

	ms_lexerlist_corrector_arrows(shell);
	ms_lexerlist_corrector_amps_pipes(shell);
	tmp = shell->st;
	shell->st = ft_itoa(shell->status);
	if (!shell->st)
		shell->st = tmp;
	else
		free(tmp);
}
