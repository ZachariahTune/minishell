/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree	*ms_parse_cmd_redin(t_shell *shell);
static t_tree	*ms_parse_cmd_redin_dbl(t_shell *shell);
static t_tree	*ms_parse_cmd_redout(t_shell *shell);
static t_tree	*ms_parse_cmd_redout_dbl(t_shell *shell);

t_tree	*ms_parse_cmd(t_shell *shell)
{
	t_lexer	*save;
	t_tree	*node;

	save = shell->templexer;
	node = ms_parse_cmd_redin(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	node = ms_parse_cmd_redin_dbl(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	node = ms_parse_cmd_redout(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	node = ms_parse_cmd_redout_dbl(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	node = ms_parse_cmdname(shell);
	if (node != NULL)
		return (node);
	return (NULL);
}

static t_tree	*ms_parse_cmd_redin(t_shell *shell)
{
	t_tree	*redirect_node;
	t_tree	*result;
	char	*lexer_value;

	redirect_node = ms_parse_cmdname(shell);
	if (redirect_node == NULL)
		return (NULL);
	if (!ms_parse_lexertype(shell, '<', NULL))
	{
		ms_parse_node_free(redirect_node);
		return (NULL);
	}
	if (!ms_parse_lexertype(shell, -1, &lexer_value))
	{
		ms_parse_node_free(redirect_node);
		return (NULL);
	}	
	result = ms_tree_malloc(shell);
	result->type = TREE_REDIN;
	if (lexer_value != NULL)
		result->value = lexer_value;
	result->left = NULL;
	result->right = ms_parse_cmd_nextarrow(shell, redirect_node);
	return (result);
}

static t_tree	*ms_parse_cmd_redin_dbl(t_shell *shell)
{
	t_tree	*redirect_node;
	t_tree	*result;
	char	*lexer_value;

	redirect_node = ms_parse_cmdname(shell);
	if (redirect_node == NULL)
		return (NULL);
	if (!ms_parse_lexertype(shell, LEXER_TYPE_DLBIN, NULL))
	{
		ms_parse_node_free(redirect_node);
		return (NULL);
	}
	if (!ms_parse_lexertype(shell, -1, &lexer_value))
	{
		ms_parse_node_free(redirect_node);
		return (NULL);
	}
	result = ms_tree_malloc(shell);
	result->type = TREE_DBLIN;
	if (lexer_value != NULL)
		result->value = lexer_value;
	result->left = NULL;
	result->right = ms_parse_cmd_nextarrow(shell, redirect_node);
	return (result);
}

static t_tree	*ms_parse_cmd_redout(t_shell *shell)
{
	t_tree	*redirect_node;
	t_tree	*result;
	char	*lexer_value;

	redirect_node = ms_parse_cmdname(shell);
	if (redirect_node == NULL)
		return (NULL);
	if (!ms_parse_lexertype(shell, '>', NULL))
	{
		ms_parse_node_free(redirect_node);
		return (NULL);
	}
	if (!ms_parse_lexertype(shell, -1, &lexer_value))
	{
		ms_parse_node_free(redirect_node);
		return (NULL);
	}	
	result = ms_tree_malloc(shell);
	result->type = TREE_REDOUT;
	if (lexer_value != NULL)
		result->value = lexer_value;
	result->left = NULL;
	result->right = ms_parse_cmd_nextarrow(shell, redirect_node);
	return (result);
}

static t_tree	*ms_parse_cmd_redout_dbl(t_shell *shell)
{
	t_tree	*redirect_node;
	t_tree	*result;
	char	*lexer_value;

	redirect_node = ms_parse_cmdname(shell);
	if (redirect_node == NULL)
		return (NULL);
	if (!ms_parse_lexertype(shell, LEXER_TYPE_DLBOUT, NULL))
	{
		ms_parse_node_free(redirect_node);
		return (NULL);
	}
	if (!ms_parse_lexertype(shell, -1, &lexer_value))
	{
		ms_parse_node_free(redirect_node);
		return (NULL);
	}
	result = ms_tree_malloc(shell);
	result->type = TREE_DBLOUT;
	if (lexer_value != NULL)
		result->value = lexer_value;
	result->left = NULL;
	result->right = ms_parse_cmd_nextarrow(shell, redirect_node);
	return (result);
}
