/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_trlist_malloc_first(t_shell *shell, t_tree *node)
{
	shell->trlist = (t_trlist *)malloc(sizeof(t_trlist));
	if (shell->trlist == NULL)
	{
		ft_free((void **)&node);
		ft_puterror(shell, 2, "(spisok uzlov lexer-dereva).\n");
	}
	shell->trlist->node = node;
	shell->trlist->clear = false;
	shell->trlist->next = NULL;
	shell->temptrlist = shell->trlist;
	node->list = shell->trlist;
}

static void	ms_trlist_malloc_next(t_shell *shell, t_tree *node)
{
	shell->temptrlist->next = (t_trlist *)malloc(sizeof(t_trlist));
	if (shell->temptrlist->next == NULL)
	{
		ft_free((void **)&node);
		ft_puterror(shell, 2, "(spisok uzlov lexer-dereva).\n");
	}
	shell->temptrlist = shell->temptrlist->next;
	shell->temptrlist->node = node;
	shell->temptrlist->clear = false;
	shell->temptrlist->next = NULL;
	node->list = shell->temptrlist;
}

t_tree	*ms_tree_malloc(t_shell *shell)
{
	t_tree	*node;

	node = (t_tree *)malloc(sizeof(t_tree));
	if (node == NULL)
		ft_puterror(shell, 2, "(lexer-derevo).\n");
	if (shell->trlist != NULL)
		ms_trlist_malloc_next(shell, node);
	if (shell->trlist == NULL)
		ms_trlist_malloc_first(shell, node);
	return (node);
}

bool	ms_parse_lexertype(t_shell *shell, int lexertype, char **bufptr)
{
	if (shell->templexer == NULL)
		return (false);
	if (shell->templexer->type == lexertype)
	{
		if (bufptr != NULL)
			*bufptr = shell->templexer->value;
		shell->templexer = shell->templexer->next;
		return (true);
	}
	shell->templexer = shell->templexer->next;
	return (false);
}

int	ms_lexerlist_parse(t_shell *shell)
{
	if (shell->output_error != 0)
		return (-1);
	if (shell->lexercount == 0)
	{
		if (shell->lexerlist != NULL)
		{
			ft_putstr(COLOR_RED, 2);
			ft_putstr("Error: Sintaksicheskaya oshibka\n", 2);
			ft_putstr(COLOR_RESET, 2);
		}
		return (-1);
	}
	shell->templexer = shell->lexerlist;
	shell->lexertree = ms_parse_cmdline(shell);
	if (shell->templexer != NULL && shell->templexer->type != 0)
	{
		ft_putstr(COLOR_RED, 2);
		ft_putstr("Error: Sintaksicheskaya oshibka \"", 2);
		ft_putstr(shell->templexer->value, 2);
		ft_putstr("\"\n", 2);
		ft_putstr(COLOR_RESET, 2);
		shell->output_error = 1;
		return (-1);
	}
	return (0);
}
