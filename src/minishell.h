/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdbool.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>

# define MSH_DEFNAME			"MiniShell"
# define MS_TEST_REGIME			0 // 1 - Test info, 0 - No test info
# define MS_READLINE_REGIME		1 // 1 - ReadLine (history), 2 - GNL (no leaks)
# define COLOR_GREEN			"\x1b[38;5;118m"
# define COLOR_RED				"\x1b[38;5;196m"
# define COLOR_ORANGE			"\x1b[38;5;208m"
# define COLOR_BLUE				"\x1b[1;40;36m"
# define COLOR_RESET			"\x1b[0m"
# define LEXER_STATE_DEFAULT	0
# define LEXER_STATE_DQUOTES	1
# define LEXER_STATE_QUOTES		2
# define LEXER_TYPE_DLBIN		301
# define LEXER_TYPE_DLBOUT		302
# define LEXER_TYPE_DLBAMP		303
# define LEXER_TYPE_DLBPIPE		304
# define LEXER_TYPE_ERROR		404
# define TREE_SEM				1
# define TREE_AMP				2
# define TREE_PIPE				3
# define TREE_REDIN				4
# define TREE_DBLIN				5
# define TREE_REDOUT			6
# define TREE_DBLOUT			7
# define TREE_CMD				8
# define TREE_ARG				9

typedef struct s_lexer	t_lexer;
typedef struct s_tree	t_tree;
typedef struct s_trlist	t_trlist;
typedef struct s_cmd	t_cmd;
typedef struct s_shell	t_shell;
extern t_shell			*g_shell;

typedef struct s_lexer
{
	int			type;
	char		*value;
	t_lexer		*next;
}	t_lexer;

typedef struct s_tree
{
	int			type;
	char		*value;
	t_trlist	*list;
	t_tree		*left;
	t_tree		*right;
}	t_tree;

typedef struct s_trlist
{
	t_tree		*node;
	bool		clear;
	t_trlist	*next;
}	t_trlist;

typedef struct s_cmd
{
	int			argc;
	char		**argv;
	bool		async;
	bool		stdin_pipe;
	bool		stdout_pipe;
	int			pipe_read;
	int			pipe_write;
	char		*redirect_in;
	char		*redirect_out;
	char		*redirect_dblin;
	char		*redirect_dblout;
	int			heredoc_fd;
	char		*heredoc_file;
}	t_cmd;

typedef struct s_shell
{
	int			output_error;
	char		*prompt_name;
	char		*prompt_line;
	char		*input;
	int			inputlen;
	int			input_i;
	int			input_c;
	int			lexerstate;
	int			lexercount;
	t_lexer		*lexerlist;
	t_lexer		*templexer;
	int			lexval_i;
	t_tree		*lexertree;
	t_trlist	*trlist;
	t_trlist	*temptrlist;
	t_cmd		*cmd;
	char		**envp;
	int			env_size;
	int			status;
	char		*st;
	pid_t		kill_pid;
}	t_shell;

//puts.c
void	ft_putchar(int c);
void	ft_putstr(char *str, int std_var);
void	ft_puterror(t_shell *shell, int code, char *name);
void	ft_puterror_noexit(t_shell *shell, int code);
void	ft_putexit(t_shell *shell);

// utils.c
void	ms_shell_starterror(int argc, char **argv);
t_shell	*ms_shell_init(void);
void	ms_readline_and_lexerlist(t_shell *shell);
void	ms_shell_destroy(t_shell *shell);

//signals.c
void	ms_signals_handler(t_shell *shell, int type, pid_t pid);

//lexer
int		ms_lexer_chartype(int last, int c, int next);
t_lexer	*ms_lexerlist_add(t_shell *shell, int value_length);
void	ms_lexerlist_build(t_shell *shell);
void	ms_lexer_parser(t_shell *shell);
void	ms_lexerlist_corrector(t_shell *shell);
void	ms_lexerlist_destroy(t_shell *shell);
char	**envp_cpy(char **env, t_shell *shell);
int		ft_str_real_len(char *s, t_shell *shell);
void	ms_lexerlist_replace_var(t_shell *shell);
int		ms_check_dquotes(t_shell *shell, const char *input, int i);

//parser
t_tree	*ms_tree_malloc(t_shell *shell);
bool	ms_parse_lexertype(t_shell *shell, int lexertype, char **bufptr);
int		ms_lexerlist_parse(t_shell *shell);
t_tree	*ms_parse_cmdline(t_shell *shell);
t_tree	*ms_parse_cmdline_pipe(t_shell *shell);
t_tree	*ms_parse_cmd(t_shell *shell);
t_tree	*ms_parse_cmdname(t_shell *shell);
t_tree	*ms_parse_cmd_nextarrow(t_shell *shell, t_tree *file_node);
void	ms_parse_node_free(t_tree *node);
void	ms_parse_tree_destroy(t_shell *shell);
void	ms_trlist_destroy(t_shell *shell);

//executor
void	ms_tree_execute(t_shell *shell);
void	ms_cmd_argv_free(t_cmd *cmd);
void	ms_tree_execute_cmd(t_shell *shell, t_tree *node, int p_r, int p_w);
void	ms_tree_execute_cmd_hlp(t_shell *shell, bool async, bool p_i, bool p_o);
void	ms_tree_execute_nextarrow(t_shell *shell, t_tree *node);
void	ms_tree_execute_args(t_shell *shell, t_tree *node, t_tree *temp, int i);
void	ms_cmd_execute(t_shell *shell);
void	ms_cmd_execute_fork(t_shell *shell);
void	ms_create_heredoc_file(t_shell *shell);
void	ms_write_heredoc_file_readline(t_shell *shell);
void	ms_write_heredoc_file_gnl(t_shell *shell, int stop);
void	ms_cmd_execute_fork_error(void);
void	ms_cmd_execute_fd_null(t_shell *shell);
void	ms_cmd_execute_fd_redirect_in(t_shell *shell);
void	ms_cmd_execute_fd_redirect_out(t_shell *shell);
void	ms_cmd_execute_command_error(t_shell *shell, int tempfd_stdout);
void	do_shell_command(t_shell *shell);
void	count_status(t_shell *shell, int status);

//commands
void	ms_cmd_execute_cd(t_shell *shell);
void	ms_cmd_execute_prompt(t_shell *shell);
void	ms_prompt_name(t_shell *shell, char *name);
void	ms_prompt_cwd(t_shell *shell);
void	ms_cmd_execute_pwd(t_shell *shell);
void	ms_cmd_execute_exit(t_shell *shell);
void	ms_cmd_execute_test(t_shell *shell);
void	ms_cmd_execute_export(t_shell *shell);
void	ms_cmd_execute_env(t_shell *shell);
void	ms_cmd_execute_unset(t_shell *shell);
void	ms_cmd_execute_echo(t_shell *shell);
int		count_correct_var(t_shell *shell, char **argv, int argc);
int		check_correct_var(t_shell *shell, char *s, char flg);
int		find_variable(char **env, char *s);
int		add_var(char ***new_env, char *s, int j, int index);
int		app_end_var(char ***new_env, char *s, int j, int index);
void	delete_if_need(char *s);
char	**copy_env_for_export(char **env, int size);
void	ft_print_next(char *s);

//test-info
void	ms_lexerlist_print(t_shell *shell);
void	ms_lexertree_print(t_shell *shell);
void	ms_cmd_print(t_cmd *cmd);

#endif
