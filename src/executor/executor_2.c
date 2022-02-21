/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_tree_execute_args(t_shell *shell, t_tree *node, t_tree *temp, int i)
{
	if (node == NULL || node->type != TREE_CMD)
	{
		shell->cmd->argc = 0;
		return ;
	}
	while (temp != NULL && (temp->type == TREE_ARG || temp->type == TREE_CMD))
	{
		temp = temp->right;
		i++;
	}
	shell->cmd->argv = (char **)malloc(sizeof(char *) * (i + 1));
	if (shell->cmd->argv == NULL)
		ft_puterror(shell, 2, "(massiv cmd->argv).\n");
	temp = node;
	i = 0;
	while (temp != NULL && (temp->type == TREE_ARG || temp->type == TREE_CMD))
	{
		shell->cmd->argv[i] = temp->value;
		temp = temp->right;
		i++;
	}
	shell->cmd->argv[i] = NULL;
	shell->cmd->argc = i;
}

void	ms_cmd_execute(t_shell *shell)
{
	if (ft_strcmp(shell->cmd->argv[0], "cd") == 0 && shell->cmd->async == 0)
		return (ms_cmd_execute_cd(shell));
	if (ft_strcmp(shell->cmd->argv[0], "prompt") == 0 && shell->cmd->async == 0)
		return (ms_cmd_execute_prompt(shell));
	if (ft_strcmp(shell->cmd->argv[0], "export") == 0 && shell->cmd->async == 0)
		return (ms_cmd_execute_export(shell));
	if (ft_strcmp(shell->cmd->argv[0], "unset") == 0 && shell->cmd->async == 0)
		return (ms_cmd_execute_unset(shell));
	if (ft_strcmp(shell->cmd->argv[0], "exit") == 0 && shell->cmd->async == 0)
	{
		ms_cmd_execute_exit(shell);
		ms_cmd_argv_free(shell->cmd);
		ms_shell_destroy(shell);
		exit(0);
	}
	ms_cmd_execute_fork(shell);
}

static void	ms_tree_execute_simplecmd(t_shell *shell, t_tree *node,
	char *redirect_in, char *redirect_out)
{
	shell->cmd->redirect_in = redirect_in;
	shell->cmd->redirect_out = redirect_out;
	shell->cmd->redirect_dblin = NULL;
	shell->cmd->redirect_dblout = NULL;
	shell->cmd->heredoc_fd = -1;
	if (shell->cmd->heredoc_file != NULL)
	{
		unlink(shell->cmd->heredoc_file);
		ft_free((void **)&shell->cmd->heredoc_file);
	}
	shell->cmd->heredoc_file = NULL;
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
	shell->cmd->redirect_in = NULL;
	shell->cmd->redirect_out = NULL;
	shell->cmd->redirect_dblin = redirect_dblin;
	shell->cmd->redirect_dblout = redirect_dblout;
	shell->cmd->heredoc_fd = -1;
	if (shell->cmd->heredoc_file != NULL)
	{
		unlink(shell->cmd->heredoc_file);
		ft_free((void **)&shell->cmd->heredoc_file);
	}
	shell->cmd->heredoc_file = NULL;
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

void	ms_tree_execute_cmd(t_shell *shell, t_tree *node, int p_r, int p_w)
{
	if (shell->cmd == NULL || node == NULL)
		return ;
	shell->cmd->pipe_read = p_r;
	shell->cmd->pipe_write = p_w;
	if (node->type == TREE_REDIN)
		ms_tree_execute_simplecmd(shell, node->right, node->value, NULL);
	if (node->type == TREE_REDOUT)
		ms_tree_execute_simplecmd(shell, node->right, NULL, node->value);
	if (node->type == TREE_DBLIN)
		ms_tree_execute_simplecmd_dbl(shell, node->right, node->value, NULL);
	if (node->type == TREE_DBLOUT)
		ms_tree_execute_simplecmd_dbl(shell, node->right, NULL, node->value);
	if (node->type == TREE_CMD)
		ms_tree_execute_simplecmd(shell, node, NULL, NULL);
}
