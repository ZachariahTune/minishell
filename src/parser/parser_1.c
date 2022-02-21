/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree	*ms_parse_cmdline_sem_before(t_shell *shell);
static t_tree	*ms_parse_cmdline_sem_after(t_shell *shell);
static t_tree	*ms_parse_cmdline_amp_before(t_shell *shell);
static t_tree	*ms_parse_cmdline_amp_after(t_shell *shell);

t_tree	*ms_parse_cmdline(t_shell *shell)
{
	t_lexer	*save;
	t_tree	*node;

	save = shell->templexer;
	node = ms_parse_cmdline_sem_before(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	node = ms_parse_cmdline_sem_after(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	node = ms_parse_cmdline_amp_before(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	node = ms_parse_cmdline_amp_after(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	node = ms_parse_cmdline_pipe(shell);
	if (node != NULL)
		return (node);
	return (NULL);
}

static t_tree	*ms_parse_cmdline_sem_before(t_shell *shell)
{
	t_tree	*pipe_node;
	t_tree	*cmd_node;
	t_tree	*result;

	pipe_node = ms_parse_cmdline_pipe(shell);
	if (pipe_node == NULL)
		return (NULL);
	if (!ms_parse_lexertype(shell, ';', NULL))
	{
		ms_parse_node_free(pipe_node);
		return (NULL);
	}
	cmd_node = ms_parse_cmdline(shell);
	if (cmd_node == NULL)
	{
		ms_parse_node_free(pipe_node);
		return (NULL);
	}
	result = ms_tree_malloc(shell);
	result->type = TREE_SEM;
	result->value = NULL;
	result->left = pipe_node;
	result->right = cmd_node;
	return (result);
}

static t_tree	*ms_parse_cmdline_sem_after(t_shell *shell)
{
	t_tree	*pipe_node;
	t_tree	*result;

	pipe_node = ms_parse_cmdline_pipe(shell);
	if (pipe_node == NULL)
		return (NULL);
	if (!ms_parse_lexertype(shell, ';', NULL))
	{
		ms_parse_node_free(pipe_node);
		return (NULL);
	}
	result = ms_tree_malloc(shell);
	result->type = TREE_SEM;
	result->value = NULL;
	result->left = pipe_node;
	result->right = NULL;
	return (result);
}

static t_tree	*ms_parse_cmdline_amp_before(t_shell *shell)
{
	t_tree	*pipe_node;
	t_tree	*cmd_node;
	t_tree	*result;

	pipe_node = ms_parse_cmdline_pipe(shell);
	if (pipe_node == NULL)
		return (NULL);
	if (!ms_parse_lexertype(shell, '&', NULL))
	{
		ms_parse_node_free(pipe_node);
		return (NULL);
	}
	cmd_node = ms_parse_cmdline(shell);
	if (cmd_node == NULL)
	{
		ms_parse_node_free(pipe_node);
		return (NULL);
	}
	result = ms_tree_malloc(shell);
	result->type = TREE_AMP;
	result->value = NULL;
	result->left = pipe_node;
	result->right = cmd_node;
	return (result);
}

static t_tree	*ms_parse_cmdline_amp_after(t_shell *shell)
{
	t_tree	*pipe_node;
	t_tree	*result;

	pipe_node = ms_parse_cmdline_pipe(shell);
	if (pipe_node == NULL)
		return (NULL);
	if (!ms_parse_lexertype(shell, '&', NULL))
	{
		ms_parse_node_free(pipe_node);
		return (NULL);
	}
	result = ms_tree_malloc(shell);
	result->type = TREE_AMP;
	result->value = NULL;
	result->left = pipe_node;
	result->right = NULL;
	return (result);
}
