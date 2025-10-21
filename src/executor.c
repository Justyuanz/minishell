#include "minishell.h"

/* Overview:
if you need to get something (command, redir, env etc),
just use the helper get_*() functions below with an index.

Example usage:
     
t_cmd *cmd = get_cmd(d, index);
t_redir *r = get_redir(cmd, index)
t_env *env = get_env(d, index);
*/

/* Data flow:

line (user input)
↓
tokenizer()           fills d->vec_tok  (tokens)
↓
build_vec_cmds()      fills d->vec_cmds (commands)
↓
executor()            reads vec_cmds and prints info
*/

/*Data structure

t_data
├── vec_tok   → [t_token*, t_token*, ...]
├── vec_cmds  → [t_cmd*, t_cmd*, ...]
│                 ├── argv[]   → ["echo", "hi", NULL]
│                 └── redirs[] → [{REDIR_OUT, "out.txt"}]
└── vec_env   → [t_env*, t_env*, ...]
|				  ├── key   ("PATH")
|                 └── value ("/usr/bin:/bin")		

t_data
- arena_tok:  stores token + argv strings (raw bytes)
- arena_env:  stores environment strings and structs (raw bytes)
- vec_tok:    vector of t_token* (all parsed tokens)(pointers to arena_tok)
- vec_env:    vector of t_env* (env list)(pointers to arena_env)
- vec_cmds:   vector of t_cmd* (each command before pipes)(pointers to arena_tok)

t_cmd
- argv:   NULL-terminated array of char* (command + args)
- redirs: vec of t_redir structs
- is_builtin: true if command is builtin

t_redir
- type:   REDIR_IN, REDIR_OUT, APPEND, HEREDOC
- file:   filename or heredoc limiter

t_token
- str:  token text
- type: WORD, PIPE, REDIR_IN, etc.

t_env
- key / value pair for environment variables
*/

/*
static void read_env_example(t_data *d)
{
	t_env *env;
	size_t i;

	i = 0;
	// loop through each element in vec_env
	while (i < d->vec_env.len)
	{
		// get the t_env struct pointer at index i
		env = get_env(d, i);
		// print its key and value in "key=value" format
		fprintf(stderr, "  env[%zu]: %s=%s\n", i, env->key, env->value);
		i++;
	}

	// When you want to find an environment variable (e.g. "$PATH"),
 	// you’ll need to loop through d->vec_env and compare the 'key'
	// if (ft_strcmp(env->key, "PATH") == 0)
 	//    return (env->value);

}
*/

void executor(t_data *d)
{
	t_cmd	*cmd; // one command node(argv + redirs)
	t_redir *redir; // single redirection
 	size_t i; // command index
	size_t j; // argv index
	size_t k; // redir index

	i = 0;
	// loop through all parsed commands (one per pipe segment)
	while (i < get_cmd_count(d))
	{
		cmd = get_cmd(d, i); // get current command from vec_cmds
        fprintf(stderr, "cmd[%zu]:\n", i);


		//Print argv for this command
		// cmd->argv is a NULL-terminated array of char*
		// Example: ["echo", "hi", NULL]
		j = 0;
        while (cmd->argv && cmd->argv[j])
		{
            fprintf(stderr, "  argv[%zu]: %s\n", j, cmd->argv[j]);
			j++;
		}

		// Print redirections belonging to this command
		// each t_redir contains:
		// type: (REDIR_IN, REDIR_OUT, APPEND, HEREDOC)
		// file: filename or limiter string
		k = 0;
		while (k < cmd->redirs.len)
		{
			redir = get_redir(cmd, k); // helper to fetch one redir
			fprintf(stderr, "  redir[%zu]: type=%s file=%s\n", k, tok_type(redir->type), redir->file); // tok_type() converts enum to readable string

			k++;
		}
		i++;
		fprintf(stderr,"--------------------------------------\n");
	}
	//read_env_example(d);
}

/* What’s next(parsing side):
need to handle syntax errors and general error cases,
test more edge inputs (weird pipes, quotes, missing filenames),
add cleanup & free,
implement env set, unset, export,
maybe start signal handling then?
*/