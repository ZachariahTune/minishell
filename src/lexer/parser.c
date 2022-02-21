/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_lexer_parser_defualt_separate(t_shell *shell, int c, int dblsep)
{
	int	value_length;

	value_length = shell->inputlen - shell->input_i;
	if (c == -3)
		shell->templexer->value[shell->lexval_i++]
			= shell->input[shell->input_i];
	if (shell->lexval_i > 0)
	{
		shell->templexer->value[shell->lexval_i] = '\0';
		shell->templexer->next = ms_lexerlist_add(shell, value_length);
		shell->templexer = shell->templexer->next;
		shell->lexval_i = 0;
	}
	if (dblsep == 1)
	{
		shell->templexer->type = shell->input_c;
		shell->templexer->value[0] = shell->input_c;
		shell->templexer->value[1] = '\0';
		shell->templexer->next = ms_lexerlist_add(shell, value_length);
		shell->templexer = shell->templexer->next;
	}
	if (c == -2)
		shell->templexer->value[shell->lexval_i++]
			= shell->input[shell->input_i];
}

static void	ms_lexer_parser_defualt(t_shell *shell, int c)
{
	if (c == '\'' || c == '\"' || c == '\\' || c == -1 || c == -2 || c == -3)
		shell->templexer->type = -1;
	if (c == '\'')
		shell->lexerstate = LEXER_STATE_QUOTES;
	else if (c == '\"')
		shell->lexerstate = LEXER_STATE_DQUOTES;
	else if (c == '\\')
	{
		shell->input_i++;
		shell->templexer->value[shell->lexval_i] = shell->input[shell->input_i];
		shell->lexval_i++;
	}
	else if (c == -1)
	{
		shell->templexer->value[shell->lexval_i] = shell->input[shell->input_i];
		shell->lexval_i++;
	}
	else if (c == ';' || c == '>' || c == '<' || c == '&' || c == '|')
		ms_lexer_parser_defualt_separate(shell, c, 1);
	else if (c == ' ' || c == -2 || c == -3)
		ms_lexer_parser_defualt_separate(shell, c, 0);
}

static void	ms_lexer_parser_dquotes(t_shell *shell)
{
	if (shell->input_c == '\\' && shell->input[shell->input_i + 1] == '\"')
	{
		shell->input_i++;
		shell->templexer->value[shell->lexval_i] = shell->input[shell->input_i];
		shell->lexval_i++;
	}
	else if (shell->input_c != '\"')
	{
		shell->templexer->value[shell->lexval_i] = shell->input[shell->input_i];
		shell->lexval_i++;
	}
	else if (shell->input_c == '\"')
	{
		shell->lexerstate = LEXER_STATE_DEFAULT;
		if (!ms_check_dquotes(shell, shell->input, shell->input_i))
			return ;
		shell->templexer->value[shell->lexval_i] = 0;
		shell->lexval_i++;
	}
}

static void	ms_lexer_parser_quotes(t_shell *shell)
{
	if (shell->input_c != '\'')
	{
		shell->templexer->value[shell->lexval_i] = shell->input[shell->input_i];
		shell->lexval_i++;
	}
	else if (shell->input_c == '\'')
	{
		shell->lexerstate = LEXER_STATE_DEFAULT;
		if (!ms_check_dquotes(shell, shell->input, shell->input_i))
			return ;
		shell->templexer->value[shell->lexval_i] = 0;
		shell->lexval_i++;
	}
}

void	ms_lexer_parser(t_shell *shell)
{
	shell->input_i = 0;
	shell->lexval_i = 0;
	shell->lexerstate = LEXER_STATE_DEFAULT;
	shell->templexer = shell->lexerlist;
	while (shell->input[shell->input_i] != '\0')
	{
		if (shell->input_i == 0)
			shell->input_c = ms_lexer_chartype(0, shell->input[shell->input_i],
					shell->input[shell->input_i + 1]);
		if (shell->input_i > 0)
			shell->input_c = ms_lexer_chartype(shell->input[shell->input_i - 1],
					shell->input[shell->input_i],
					shell->input[shell->input_i + 1]);
		if (shell->lexerstate == LEXER_STATE_DEFAULT)
			ms_lexer_parser_defualt(shell, shell->input_c);
		else if (shell->lexerstate == LEXER_STATE_DQUOTES)
			ms_lexer_parser_dquotes(shell);
		else if (shell->lexerstate == LEXER_STATE_QUOTES)
			ms_lexer_parser_quotes(shell);
		if (shell->input[shell->input_i + 1] == '\0' && shell->lexval_i > 0)
			shell->templexer->value[shell->lexval_i] = '\0';
		shell->input_i++;
	}
}
