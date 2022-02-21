/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_lexer_chartype(int last, int c, int next)
{
	if (c == '\'' || c == '\"' || c == '\\' || c == ';'
		|| c == ' ' || c == '\t' || c == '\n' || c == '\0'
		|| (c == '>' && last != '>' && next != '>')
		|| (c == '<' && last != '<' && next != '<')
		|| (c == '&' && last != '&' && next != '&')
		|| (c == '|' && last != '|' && next != '|'))
		return (c);
	else if ((c == '>' && last != '>' && next == '>')
		|| (c == '<' && last != '<' && next == '<')
		|| (c == '&' && last != '&' && next == '&')
		|| (c == '|' && last != '|' && next == '|'))
		return (-2);
	else if ((c == '>' && last == '>' && next != '>')
		|| (c == '<' && last == '<' && next != '<')
		|| (c == '&' && last == '&' && next != '&')
		|| (c == '|' && last == '|' && next != '|'))
		return (-3);
	else
		return (-1);
}

t_lexer	*ms_lexerlist_add(t_shell *shell, int value_length)
{
	t_lexer	*lexerlist;

	lexerlist = (t_lexer *)malloc(sizeof(t_lexer));
	if (lexerlist == NULL)
	{
		ft_puterror(shell, 2, "(element lexera).\n");
	}
	lexerlist->value = (char *)malloc(value_length + 1);
	if (lexerlist->value == NULL)
	{
		ft_free((void **)&lexerlist);
		ft_puterror(shell, 2, "(znachenie elementa lexera).\n");
	}
	lexerlist->value[0] = '\0';
	lexerlist->type = '\0';
	lexerlist->next = NULL;
	return (lexerlist);
}

static void	ms_lexer_glob(t_shell *shell, int files_count, char ***files)
{
	t_lexer	*lexerlist_saved;
	int		i;

	shell->lexercount += files_count;
	lexerlist_saved = shell->templexer->next;
	ft_free((void **)&shell->templexer->value);
	shell->templexer->value = (char *)malloc(ft_strlen((*files)[0]) + 1);
	if (shell->templexer->value == NULL)
	{
		ft_arrayfree((void ***)files, files_count);
		ft_puterror(shell, 2, "(znachenie elementa lexera).\n");
	}
	ft_strcpy(shell->templexer->value, (*files)[0]);
	i = 0;
	while (++i < files_count)
	{
		shell->templexer->next
			= ms_lexerlist_add(shell, ft_strlen((*files)[i]));
		shell->templexer = shell->templexer->next;
		shell->templexer->type = -1;
		ft_strcpy(shell->templexer->value, (*files)[i]);
	}
	shell->templexer->next = lexerlist_saved;
}

void	ms_lexerlist_build(t_shell *shell)
{
	int		files_count;
	char	**files;

	shell->lexerlist = ms_lexerlist_add(shell, shell->inputlen);
	ms_lexer_parser(shell);
	shell->templexer = shell->lexerlist;
	while (shell->templexer != NULL)
	{
		if (shell->templexer->type == -1)
		{
			files_count = ft_miniglob(shell->templexer->value, &files);
			if (files_count == -2)
				ft_puterror_noexit(shell, 1);
			if (files_count == -1)
				ft_puterror(shell, 2, "(funkciya miniglob).\n");
			if (files_count == 0)
				shell->lexercount++;
			if (files_count > 0)
				ms_lexer_glob(shell, files_count, &files);
			if (files_count >= 0)
				ft_arrayfree((void ***)&files, files_count);
		}
		shell->templexer = shell->templexer->next;
	}
}
