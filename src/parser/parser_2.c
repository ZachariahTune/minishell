/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree	*ms_parse_cmdline_pipe_hlp(t_shell *shell);

t_tree	*ms_parse_cmdline_pipe(t_shell *shell)
{
	t_lexer	*save;
	t_tree	*node;

	save = shell->templexer;
	node = ms_parse_cmdline_pipe_hlp(shell);
	if (node != NULL)
		return (node);
	shell->templexer = save;
	node = ms_parse_cmd(shell);
	if (node != NULL)
		return (node);
	return (NULL);
}

static t_tree	*ms_parse_cmdline_pipe_hlp(t_shell *shell)
{
	t_tree	*cmd_node;
	t_tree	*pipe_node;
	t_tree	*result;

	cmd_node = ms_parse_cmd(shell);
	if (cmd_node == NULL)
		return (NULL);
	if (!ms_parse_lexertype(shell, '|', NULL))
	{
		ms_parse_node_free(cmd_node);
		return (NULL);
	}
	pipe_node = ms_parse_cmdline_pipe(shell);
	if (pipe_node == NULL)
	{
		ms_parse_node_free(cmd_node);
		return (NULL);
	}
	result = ms_tree_malloc(shell);
	result->type = TREE_PIPE;
	result->value = NULL;
	result->left = cmd_node;
	result->right = pipe_node;
	return (result);
}
