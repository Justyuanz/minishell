#ifndef MINISHELL_H
# define MINISHELL_H

#include "vec.h"
#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	APPEND,
	REDIR_OUT,
	HEREDOC,
	ENV,
	EXPAND
}	t_token_type;

typedef enum e_builtin_type
{
	BUILTIN_CD = 11,
	BUILTIN_ECHO,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT
}	t_builtin_type;

typedef struct s_quote
{
	bool single_ON;
	bool double_ON;
}	t_quote;

typedef struct s_arena
{
	char	*data;
	size_t	capacity;
	size_t	offset;
}	t_arena;

typedef	struct s_redir
{
	t_token_type	type;
	char			*file;
}	t_redir;

typedef struct s_cmd
{
	char	**argv;
	t_vec	redirs;
	bool	is_builtin;
} 	t_cmd;

typedef struct s_token
{
	char	*str;
	t_token_type type;
}	t_token;

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

typedef struct s_data
{
	t_arena	arena_tok;
	t_arena arena_env;
	t_vec	vec_tok;
	t_vec	vec_env;
	t_vec	vec_cmds;
}	t_data;

typedef struct	s_shell
{
	int exitcode;
	int	input;
	int	output;
	int	command_index;
	int	index;
	int *pids;
	int	**pipe_array;
	int	pipes_count;
	char **envp;
	t_data *data;
} t_shell;

void executor(t_data *d);
t_data *get_data(void);
size_t get_cmd_count(t_data *d);
t_cmd *get_cmd(t_data *d, size_t index);
t_redir *get_redir(t_cmd *cmd, size_t index);
//void debug_print_tokens(t_data *d); // for debugging
//void debug_print_cmds(t_data *d); //for debugging
t_token *get_tok(t_data *d, size_t index);
//t_vec *get_vec_cmd(t_data *d, size_t index);
t_env *get_env(t_data *d, size_t index);
void push_tok(t_data *d, char *line, size_t len, int type);
void tokenizer(t_data *d, char *line);
void read_the_line(t_data *d, t_shell *shell);
char *tok_type(t_token_type tok_type);
void build_vec_cmds(t_data *d);

bool ft_isspace(char c);
void shell_init(t_data *d, char **envp);
bool str_cmp(char *s1, char *s2);

int	 arena_init(t_arena *arena, size_t capacity);
void envp_init(t_data *d, char **envp);
char *arena_push(t_arena *arena, char *s, size_t len);
void *arena_alloc(t_arena *arena, size_t elem_size);
void arena_reset(t_arena *arena);
void arena_free(t_arena *arena);

size_t read_pipe(t_data *d, char *line, size_t i);
size_t read_word(t_data *d, char *line, size_t i);
size_t read_env_operator(t_data *d, char *line, size_t i);
size_t read_redir_operator2(t_data *d, char *line, size_t i);
size_t read_redir_operator(t_data *d, char *line, size_t i);

void handle_expansion(t_data *d, char *buf, char *line, size_t *i, size_t *off);

//execution.c
void    shell_execution(t_shell *shell);

//checkers.c
int	check_access(const char *full_path, int *found);

//cleaners.c
void	free_array(char **array);
void    error_smt(void);
void    free_pipes(t_shell *shell);
void    free_string(char *str);

//path.c
char	*get_command_path(const char *cmd, t_shell *shell);

//single_command.c
void    single_command_case(t_shell *shell);
void    wait_for_all(t_shell *shell);

//utils.c
int ft_strcmp(const char *s1, const char *s2);
void    error_smt(void);
void    update_exitcode(int error_code, t_shell *shell);
char *get_env_value(t_shell *shell, char *str);
char	**convert_env_vec_to_array(t_shell *shell);

//builtins
void    handle_builtin(int flag, t_cmd *command, t_shell *shell);
int check_if_builtin(char *command);
void    builtin_cd(int i, char **command_array, t_shell *shell);
void    builtin_echo(t_cmd *command, t_shell *shell);
void    builtin_pwd(void);
void    builtin_env(t_shell *shell);
void	builtin_export(t_cmd *cmd, t_shell *shell);

void	builtin_exit(t_cmd *cmd, t_shell *shell);

//piping.c
int create_pipes(t_shell *shell);
void    handle_pipes(t_shell *shell);

//redirection.c
void	redirect_child(t_cmd *cmd, t_shell *shell);

//heredoc.c
int	handle_heredocs(t_cmd *cmd);


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
kill(pid, SIGTERM)				Send signal to process (used for kill or heredoc)

----Process Management----
Used to fork processes, wait for them, and execute commands.

fork()							Creates child process (returns 0 in child, >0 in parent)
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
