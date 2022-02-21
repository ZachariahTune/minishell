/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_lexerlist_print(t_shell *shell)
{
	t_lexer	*templexer;

	templexer = shell->lexerlist;
	printf("*****************************\n");
	printf("*********** LEXER ***********\n");
	printf("*****************************\n");
	printf("lexercount = %d\n", shell->lexercount);
	while (templexer->next)
	{
		printf("type = %d; value = %s\n", templexer->type, templexer->value);
		templexer = templexer->next;
	}
	printf("type = %d; value = %s\n", templexer->type, templexer->value);
}

static void	ms_lexertree_print_level(int level)
{
	int	i;

	i = 0;
	while (i < level)
	{
		printf("   ");
		i++;
	}
}

static void	ms_lexertree_print_rec(t_tree *tree, int level)
{
	if (tree != NULL)
	{
		ms_lexertree_print_level(level);
		printf("type = %d; value = %s\n", tree->type, tree->value);
		if (tree->left)
		{
			ms_lexertree_print_level(level);
			printf("-> Left\n");
			ms_lexertree_print_rec(tree->left, level + 1);
		}
		if (tree->right)
		{
			ms_lexertree_print_level(level);
			printf("-> Right\n");
			ms_lexertree_print_rec(tree->right, level + 1);
		}
	}
}

void	ms_lexertree_print(t_shell *shell)
{
	t_tree	*temptree;

	temptree = shell->lexertree;
	printf("*****************************\n");
	printf("*********** PARSER **********\n");
	printf("*****************************\n");
	ms_lexertree_print_rec(temptree, 0);
}

void	ms_cmd_print(t_cmd *cmd)
{
	int	i;

	i = 0;
	printf("argc = %d; ", cmd->argc);
	while (i < cmd->argc)
	{
		printf("argv[%d] = %s; ", i, cmd->argv[i]);
		i++;
	}
	printf("async = %d; ", (int)cmd->async);
	printf("stdin_pipe = %d; ", (int)cmd->stdin_pipe);
	printf("stdout_pipe = %d; ", (int)cmd->stdout_pipe);
	printf("pipe_read = %d; ", cmd->pipe_read);
	printf("pipe_write = %d; ", cmd->pipe_write);
	printf("redirect_in = %s; ", cmd->redirect_in);
	printf("redirect_out = %s; ", cmd->redirect_out);
	printf("redirect_dblin = %s; ", cmd->redirect_dblin);
	printf("redirect_dblout = %s\n", cmd->redirect_dblout);
}
