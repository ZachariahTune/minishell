/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_tree_execute_pipe(t_shell *shell, t_tree *node,
	t_tree *node_right, bool async)
{
	int		fd[4];

	pipe(fd);
	fd[2] = fd[0];
	fd[3] = fd[1];
	ms_tree_execute_cmd_hlp(shell, async, false, true);
	ms_tree_execute_cmd(shell, node->left, 0, fd[3]);
	while (node_right != NULL && node_right->type == TREE_PIPE)
	{
		close(fd[3]);
		pipe(fd);
		fd[3] = fd[1];
		ms_tree_execute_cmd_hlp(shell, async, true, true);
		ms_tree_execute_cmd(shell, node_right->left, fd[2], fd[3]);
		close(fd[2]);
		fd[2] = fd[0];
		node_right = node_right->right;
	}
	fd[2] = fd[0];
	close(fd[3]);
	ms_tree_execute_cmd_hlp(shell, async, true, false);
	ms_tree_execute_cmd(shell, node_right, fd[2], 0);
	close(fd[2]);
}

static void	ms_tree_execute_left(t_shell *shell, t_tree *node, bool async)
{
	if (node == NULL)
		return ;
	if (node->type == TREE_PIPE)
	{
		ms_tree_execute_pipe(shell, node, node->right, async);
	}
	else
	{
		ms_tree_execute_cmd_hlp(shell, async, false, false);
		ms_tree_execute_cmd(shell, node, 0, 0);
	}
}

static void	ms_tree_execute_cmdline(t_shell *shell, t_tree *tree)
{
	if (tree == NULL)
		return ;
	if (shell->cmd == NULL)
	{
		shell->cmd = (t_cmd *)malloc(sizeof(t_cmd));
		if (shell->cmd != NULL)
			shell->cmd->heredoc_file = NULL;
	}
	if (shell->cmd == NULL)
		ft_puterror(shell, 2, "(struktura CMD).\n");
	if (tree->type == TREE_SEM)
	{
		ms_tree_execute_left(shell, tree->left, false);
		ms_tree_execute_cmdline(shell, tree->right);
	}
	else if (tree->type == TREE_AMP)
	{
		ms_tree_execute_left(shell, tree->left, true);
		ms_tree_execute_cmdline(shell, tree->right);
	}
	else
		ms_tree_execute_left(shell, tree, false);
}

void	ms_tree_execute(t_shell *shell)
{
	t_tree	*temptree;

	if (MS_TEST_REGIME == 1)
	{
		printf("*****************************\n");
		printf("********** EXECUTOR *********\n");
		printf("*****************************\n");
	}
	temptree = shell->lexertree;
	ms_tree_execute_cmdline(shell, temptree);
}

void	ms_cmd_argv_free(t_cmd *cmd)
{
	ft_free((void **)&cmd->argv);
	cmd->argc = 0;
}
