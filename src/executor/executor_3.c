/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_tree_execute_cmd_hlp(t_shell *shell, bool async, bool p_i, bool p_o)
{
	if (shell->cmd == NULL)
		return ;
	shell->cmd->async = async;
	shell->cmd->stdin_pipe = p_i;
	shell->cmd->stdout_pipe = p_o;
}

static void	ms_tree_execute_simplecmd(t_shell *shell, t_tree *node,
	char *redirect_in, char *redirect_out)
{
	if (redirect_in != NULL)
	{
		shell->cmd->redirect_in = redirect_in;
		shell->cmd->redirect_dblin = NULL;
	}
	if (redirect_out != NULL)
	{
		shell->cmd->redirect_out = redirect_out;
		shell->cmd->redirect_dblout = NULL;
	}
	if (node != NULL && (node->type == TREE_REDIN || node->type == TREE_REDOUT
			|| node->type == TREE_DBLIN || node->type == TREE_DBLOUT))
	{
		ms_tree_execute_nextarrow(shell, node);
		return ;
	}
	ms_tree_execute_args(shell, node, node, 0);
	if (MS_TEST_REGIME == 1)
		ms_cmd_print(shell->cmd);
	if (shell->cmd->argc > 0)
		ms_cmd_execute(shell);
	ms_cmd_argv_free(shell->cmd);
}

static void	ms_tree_execute_simplecmd_dbl(t_shell *shell, t_tree *node,
	char *redirect_dblin, char *redirect_dblout)
{
	if (redirect_dblin != NULL)
	{
		shell->cmd->redirect_dblin = redirect_dblin;
		shell->cmd->redirect_in = NULL;
	}
	if (redirect_dblout != NULL)
	{
		shell->cmd->redirect_dblout = redirect_dblout;
		shell->cmd->redirect_out = NULL;
	}
	if (node != NULL && (node->type == TREE_REDIN || node->type == TREE_REDOUT
			|| node->type == TREE_DBLIN || node->type == TREE_DBLOUT))
	{
		ms_tree_execute_nextarrow(shell, node);
		return ;
	}
	ms_tree_execute_args(shell, node, node, 0);
	if (MS_TEST_REGIME == 1)
		ms_cmd_print(shell->cmd);
	if (shell->cmd->argc > 0)
		ms_cmd_execute(shell);
	ms_cmd_argv_free(shell->cmd);
}

void	ms_tree_execute_nextarrow(t_shell *shell, t_tree *node)
{
	if (node->type == TREE_REDIN)
		ms_tree_execute_simplecmd(shell, node->right, node->value, NULL);
	if (node->type == TREE_REDOUT)
		ms_tree_execute_simplecmd(shell, node->right, NULL, node->value);
	if (node->type == TREE_DBLIN)
		ms_tree_execute_simplecmd_dbl(shell, node->right, node->value, NULL);
	if (node->type == TREE_DBLOUT)
		ms_tree_execute_simplecmd_dbl(shell, node->right, NULL, node->value);
}
