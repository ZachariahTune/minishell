/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_lexerlist_free(t_lexer *lexerlist)
{
	if (lexerlist->value != NULL)
		ft_free((void **)&lexerlist->value);
	if (lexerlist->next != NULL)
		ms_lexerlist_free(lexerlist->next);
	ft_free((void **)&lexerlist);
}

void	ms_lexerlist_destroy(t_shell *shell)
{
	if (shell->lexerlist != NULL)
	{
		ms_lexerlist_free(shell->lexerlist);
		shell->lexerlist = NULL;
	}
}
