/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree	*ms_parse_cmdargs(t_shell *shell);
static t_tree	*ms_parse_cmdargs_hlp(t_shell *shell);

t_tree	*ms_parse_cmdname(t_shell *shell)
{
	t_tree	*cmdarg_node;
	t_tree	*result;
	char	*lexer_value;

	if (!ms_parse_lexertype(shell, -1, &lexer_value))
		return (NULL);
	cmdarg_node = ms_parse_cmdargs(shell);
	result = ms_tree_malloc(shell);
	result->type = TREE_CMD;
	if (lexer_value != NULL)
		result->value = lexer_value;
	result->left = NULL;
	result->right = cmdarg_node;
	return (result);
}

static t_tree	*ms_parse_cmdargs(t_shell *shell)
{
	t_lexer	*save;
	t_tree	*node;

	save = shell->templexer;
	node = ms_parse_cmdargs_hlp(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	return (NULL);
}

static t_tree	*ms_parse_cmdargs_hlp(t_shell *shell)
{
	t_tree	*cmdarg_node;
	t_tree	*result;
	char	*lexer_value;

	if (!ms_parse_lexertype(shell, -1, &lexer_value))
		return (NULL);
	cmdarg_node = ms_parse_cmdargs(shell);
	result = ms_tree_malloc(shell);
	result->type = TREE_ARG;
	if (lexer_value != NULL)
		result->value = lexer_value;
	result->left = NULL;
	result->right = cmdarg_node;
	return (result);
}

t_tree	*ms_parse_cmd_nextarrow(t_shell *shell, t_tree *file_node)
{
	t_tree	*result;

	if (shell->templexer != NULL && shell->templexer->next != NULL
		&& (shell->templexer->type == '<' || shell->templexer->type == '>'
			|| shell->templexer->type == LEXER_TYPE_DLBIN
			|| shell->templexer->type == LEXER_TYPE_DLBOUT)
		&& shell->templexer->next->type == -1)
	{
		result = ms_tree_malloc(shell);
		if (shell->templexer->type == '<')
			result->type = TREE_REDIN;
		if (shell->templexer->type == '>')
			result->type = TREE_REDOUT;
		if (shell->templexer->type == LEXER_TYPE_DLBIN)
			result->type = TREE_DBLIN;
		if (shell->templexer->type == LEXER_TYPE_DLBOUT)
			result->type = TREE_DBLOUT;
		result->value = shell->templexer->next->value;
		result->left = NULL;
		shell->templexer = shell->templexer->next->next;
		result->right = ms_parse_cmd_nextarrow(shell, file_node);
		return (result);
	}
	return (file_node);
}
