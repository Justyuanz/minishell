/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 14:25:10 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/10 14:48:44 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "vec.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>

//# include <stdio.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# define WORD_BUF_SIZE 2048

# define SYNTAX_ERROR_PIPE "mini: syntax error near unexpected token `|'"
# define SYNTAX_ERROR_NEWLINE "syntax error near unexpected token `newline'"
# define SYNTAX_ERROR_REDIR "mini: syntax error near unexpected token "
# define SYNTAX_ERROR_QUOTE "mini: unclosed quote"
# define ERROR_MSG_AMBIGUOUS "mini: ambiguous redirection : "

extern int			g_signal;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	APPEND,
	REDIR_OUT,
	HEREDOC,
	EMPTY_WORD
}					t_token_type;

typedef enum e_builtin_type
{
	BUILTIN_CD = 11,
	BUILTIN_ECHO,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT
}					t_builtin_type;

typedef struct s_quote
{
	bool			single_on;
	bool			double_on;
}					t_quote;

typedef struct s_arena
{
	char			*data;
	size_t			capacity;
	size_t			offset;
}					t_arena;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	t_quote			quote;
	bool			is_ambiguous;
	int				strdupped;
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_vec			redirs;
	t_vec			quotes;
}					t_cmd;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	t_quote			quote;
}					t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				key_dupped;
	int				value_dupped;
}					t_env;

typedef struct s_buffer
{
	char			buf[WORD_BUF_SIZE];
	char			tmp[1024];
	t_quote			quotes;

}					t_buffer;

typedef struct s_data
{
	char			*line;
	t_arena			arena_tok;
	t_arena			arena_env;
	t_vec			vec_tok;
	t_vec			vec_env;
	t_vec			vec_cmds;
	t_quote			quote;
	int				heredoc_skip;
	int				unclosed_quote;
	int				expanded_empty;
	int				heredoc_done;
}					t_data;

typedef struct s_shell
{
	int				exitcode;
	int				input;
	int				output;
	int				command_index;
	int				index;
	int				*pids;
	int				**pipe_array;
	int				pipes_count;
	char			**envp;
	bool			is_amb;
	int				savestdout;
	int				savestdin;
	int				single_builtin;
	t_data			*data;
}					t_shell;

// debug
void				debug_print_cmds(t_data *d);
void				debug_print_tokens(t_data *d);
void				executor(t_data *d);

// getters
t_data				*get_data(void);
size_t				get_cmd_count(t_data *d);
t_cmd				*get_cmd(t_data *d, size_t index);
t_redir				*get_redir(t_cmd *cmd, size_t index);
t_token				*get_tok(t_data *d, size_t index);
t_env				*get_env(t_data *d, size_t index);
t_quote				*get_quote(t_cmd *cmd, size_t index);

// utilities
bool				ft_isspace(char c);
bool				str_cmp(char *s1, char *s2);
char				*tok_type(t_token_type tok_type);

// arena & memory management
int					arena_init(t_arena *arena, size_t capacity);
void				*arena_alloc(t_data *d, t_arena *arena, size_t elem_size);
char				*arena_push(t_arena *arena, char *s, size_t len);
void				arena_reset(t_arena *arena);
void				arena_free(t_arena *arena);

void				envp_init(t_data *d, char **envp);

// tokenizer
void				push_word_tok(t_data *d, size_t len, int type,
						t_buffer *buffer);
size_t				read_word(t_data *d, t_buffer *buffer, size_t i);
size_t				read_pipe(t_data *d, size_t i);
size_t				read_redir_operator(t_data *d, size_t i);
size_t				read_redir_operator2(t_data *d, size_t i);
bool				tokenizer(t_data *d);

// input reading
void				read_heredoc_mode(t_data *d, size_t *i, size_t *off,
						t_buffer *buffer);
size_t				handle_no_quote(t_data *d, char *buf, size_t i,
						size_t *off);
size_t				handle_double_quote(t_data *d, size_t i, size_t *off,
						t_buffer *buffer);
size_t				handle_single_quote(t_data *d, size_t i, size_t *off,
						t_buffer *buffer);

// syntax & parser
bool				parse_error_msg(char *msg, char *var, int exitcode);
bool				syntax_validation(t_data *d);
void				build_vec_cmds(t_data *d);

// parser helper
void				handle_pipe(t_data *d, t_vec *argv, t_cmd *cmd);
void				handle_redir(t_data *d, t_cmd *cmd, t_token *tok,
						size_t *i);
void				is_ambigurous_redir(t_redir *redir);
void				track_quotes(t_data *d, t_token *tok, t_cmd *cmd);
size_t				count_words(char const *s);

// expansion
void				handle_expansion(t_data *d, char *buf, size_t *i,
						size_t *off);
bool				expand_in_heredoc(t_redir *redir);
void				handle_variable(t_data *d, char *buf, size_t *i,
						size_t *off);
void				store_var_name(t_data *d, char *tmp, size_t *i, size_t *j);
bool				bare_dollar(t_data *d, char *buf, size_t *off, size_t *i);
bool				exit_status(t_data *d, size_t *i, char *buf, size_t *off);

// shell loop
void				read_the_line(t_data *d, t_shell *shell);
void				shell_init(t_data *d, t_shell *shell, char **envp);

// clean up
void				cleanup_line(t_data *d);
void				cleanup_line_runtime(t_data *d);
void				cleanup_shell(t_data *d);
void				destroy_and_exit(t_data *d, char *msg, int exitcode);
void				eof_cleanup(t_data *d, t_shell *shell);

// execution.c
void				shell_execution(t_data *d, t_shell *shell);
t_shell				*ft_shell(void);

// execution_fork.c
int					handle_command(t_shell *shell, int command_count);

// checkers.c
int					check_access(const char *full_path, int *found);

// cleaners.c
void				free_array(char **array);
void				free_pipes(t_shell *shell);
void				free_string(char *str);
void				cleanup_env_vars(t_shell *shell);

// path1.c
char				*get_command_path(const char *cmd, t_shell *shell);

// path.c
char				*join_paths(const char *dir, const char *cmd);
char				*search_in_cwd(const char *cmd, t_shell *shell);

// single_command.c
void				single_command_case(t_shell *shell);
void				wait_for_all(t_shell *shell);

// utils.c
int					ft_strcmp(const char *s1, const char *s2);
void				update_exitcode(int error_code, t_shell *shell);
char				*get_env_value(t_shell *shell, char *str);
void				handle_path_error(const char *cmd, t_shell *shell,
						int found);
void				cleanup_parent(t_shell *shell);
void				final_cleaner(t_shell *shell);
void				final_exit(t_shell *shell, int exit_number);
// builtins
void				handle_builtin(int flag, t_cmd *command, t_shell *shell);
int					check_if_builtin(t_shell *shell, char *command);
void				builtin_cd(int i, char **command_array, t_shell *shell);
void				builtin_echo(t_cmd *command, t_shell *shell);
void				builtin_pwd(t_shell *shell);
void				builtin_env(t_shell *shell);
void				builtin_export(t_cmd *cmd, t_shell *shell);
void				builtin_unset(t_cmd *cmd, t_shell *shell);
void				update_shell_envp(t_shell *shell);
void				builtin_exit(t_cmd *cmd, t_shell *shell);
int					is_valid_identifier(const char *str);
void				print_exported_vars(t_shell *shell);
// int					update_env_var(t_shell *shell, char *key,
//						t_env *env_var;
// 						char *value);
// int					add_env_var(t_shell *shell, char *key, t_env *env_var,
// 						char *value);

int					update_env_var(t_shell *shell, char *key, char *value);
int					add_env_var(t_shell *shell, char *key, char *value);
void				env_to_null(t_env *env);
int					export_argv(t_cmd *cmd, t_shell *shell);

// piping.c
int					create_pipes(t_shell *shell);
void				handle_pipes(t_shell *shell);
void				close_parent_pipes(t_shell *shell);

// redirection.c
int					redirect_child(t_cmd *cmd, t_shell *shell);

// heredoc.c
int					handle_heredocs(t_data *d, t_cmd *cmd);
int					heredoc_stuff(t_data *d, t_cmd *cmd, t_shell *shell);

// heredoc_input.c
int					read_heredoc_input(t_data *d, const char *delimiter,
						char *filename, t_redir *redir);

// envp.c
char				**create_envp_from_data(t_data *data);

// signals.c
void				set_prompt_signals(void);
void				set_parent_wait_signals(void);
void				set_child_signals(void);
void				set_heredoc_signal(void);
void				heredoc_signal_handler(int g_signal);

#endif

/*

Stack (t_data)
+------------------+
| arena            |
| vec_tok {        |---> memory (malloc, heap)
|   memory ------+ |
|   len = 0      | |
|   capacity = 4 | |
| }              | |
| q              | |
+------------------+

Heap (vec_tok.memory, capacity=4)
+---------+---------+---------+---------+
| tok*    | tok*    | tok*    | tok*    |
+---------+---------+---------+---------+

Heap (arena.data)
+----------------------+-----------------+ ...
| t_token structs      | strings "..."   |
+----------------------+-----------------+ ...

*/

/*
-----Prompt & Readline-----
Used to show prompt, handle history, and update input line.

readline("minishell> ")			Show prompt and return user input as char *
add_history(line)				Add a command to history (↑ ↓ keys)
rl_replace_line("", 0)			Replace current input (used in signal handler)
rl_on_new_line()				Move cursor to new line
rl_redisplay()					Redraw prompt after interruption
rl_clear_history()				Free all history (used on cleanup)

-----Signal Handling-----
Used to make Ctrl+C and Ctrl+\ behave like in Bash.

signal(SIGINT, handler)			Handle Ctrl+C
sigaction(SIGINT, &act, NULL)	More powerful alternative to signal()
sigemptyset(&set)				Prepare empty signal set
sigaddset(&set, SIGQUIT)		Add signal to signal set
kill(pid,
					SIGTERM)				Send signal to process
										(used for kill or heredoc)

----Process Management----
Used to fork processes, wait for them, and execute commands.

fork()							Creates child process (returns 0 in child,
								>0 in parent)
execve(path, argv, envp)		Replaces current process with another program
exit(status)					Terminates current process
wait(&status)					Waits for any child
waitpid(pid, &status, 0)		Waits for specific child
wait3(), wait4()				Advanced wait with resource usage

----File I/O and Redirection----
Used to handle >, >>, <, <<, and pipes.

open(path, flags, mode)			Opens a file
read(fd, buf, size)				Read from fd
write(fd, buf, size)			Write to fd
dup(fd)							Duplicate a file descriptor
dup2(old, new)					Redirect new to old
close(fd)						Close a file descriptor
pipe(fd[2])						Create a pipe: fd[0] = read, fd[1] = write
unlink(path)					Delete file (used for heredoc temp file)

----Environment & Path----
Used for env, export, unset, cd, and finding executables.

getcwd(buf, size)				Get current directory
chdir(path)						Change current directory
getenv("PATH")					Get value of environment variable
access(path, mode)				Check if file is executable (X_OK)
stat, fstat, lstat				File metadata (used for env, . check)

----Directory Functions----
Only needed if parse files or directories (e.g., . or for cd)

opendir(path)					Opens directory stream
readdir(dir)					Reads directory entries
closedir(dir)					Closes directory stream

-----Error Handling-----
Show meaningful error messages to stderr.

perror("msg")					Print "msg: system error"
strerror(errno)					Convert error code to readable message

----TTY & Terminal Control----
For checking if user is in a real terminal or for fancy output.

isatty(fd)						Is fd a terminal?
ttyname(fd)						Get name of terminal device
ttyslot()						Get terminal slot
ioctl()							Terminal control (rarely needed)

----Termcap (for terminal formatting)----
Used to do things like clear screen, move cursor.

tgetent(buf, termname)			Load terminal capabilities
tgetflag(capname)				Get boolean cap
tgetnum(capname)				Get numeric cap
tgetstr(capname, &area)			Get string cap
tgoto(cap, col, row)			Build cursor movement string
tputs(str, affcnt, putc)		Output termcap string

*/
