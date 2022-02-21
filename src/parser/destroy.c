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

void	ms_parse_node_free(t_tree *node)
{
	if (node->left != NULL)
		ms_parse_node_free(node->left);
	if (node->right != NULL)
		ms_parse_node_free(node->right);
	if (node != NULL)
	{
		node->list->clear = true;
		ft_free((void **)&node);
	}
}

void	ms_parse_tree_destroy(t_shell *shell)
{
	if (shell->lexertree != NULL)
	{
		ms_parse_node_free(shell->lexertree);
		shell->lexertree = NULL;
	}
}

static void	ms_trlist_free(t_trlist *trlist)
{
	if (trlist->clear == false)
		ft_free((void **)&trlist->node);
	if (trlist->next != NULL)
		ms_trlist_free(trlist->next);
	ft_free((void **)&trlist);
}

void	ms_trlist_destroy(t_shell *shell)
{
	if (shell->trlist != NULL)
	{
		ms_trlist_free(shell->trlist);
		shell->trlist = NULL;
	}
}
