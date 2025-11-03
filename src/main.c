#include "minishell.h"
// Fix t_cmd to own t_vec argv
//Remove vec_new(&d->vec_cmds, ...) from init_cmd
//In shell_init, use sizeof(t_cmd *) for vec_cmds
//Add vec_free(&cmd->argv) in 
//??Call cleanup_line(d) before arena_reset(&d->arena_tok)
//Keep your arena alignment fix

void cleanup_line_runtime(t_data *d)
{
    if (!d) return;

    for (size_t i = 0; i < d->vec_cmds.len; i++) {
        t_cmd *cmd = get_cmd(d, i);

        // Free ONLY the argv array (heap). Do NOT free argv[i] strings (arena-owned).
        if (cmd->argv) {
            free(cmd->argv);
            cmd->argv = NULL;
        }

        // Free the vector buffer for redirs (its elements are arena pointers)
        vec_free(&cmd->redirs);
    }

    // Reset container lengths (forget elements) but keep their buffers for reuse.
    vec_reset(&d->vec_cmds);
    vec_reset(&d->vec_tok);
}
void pipe_misuse(t_data *d, t_token *tok, size_t i)
{
	if (i == 0 && tok->type == PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'\n", 2);
		exit (EXIT_FAILURE);
	}
	else if ((get_tok(d, (i - 1))->type == PIPE))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'\n", 2);
		exit (EXIT_FAILURE);
	}
}
void syntax_validation(t_data *d)
{
	t_token *tok;
	size_t i;

	i = 0;
	while (i < d->vec_tok.len)
	{
		tok = get_tok(d, i);
		pipe_misuse(d, tok, i);
		i++;
	}
	pipe_misuse(d, tok, i);

	/*
	bash: syntax error near unexpected token `|'
	1. Pipe Misuse (pipe found at the beginning or end)
	2. Redirection Without Target
	3. Consecutive Redirections Without Words
	4. Double Pipes or Logical Operators
	5. Unclosed Quotes
	6. Heredoc Syntax Errors
	7. Only Spaces / Empty Input
	8. Random Forbidden Symbols

	*/
}

void read_the_line(t_data *d)
{
    char  *line;

    line = readline("minishell$");
    if (!line)
    {
        //cleanup_shell(d);
        ft_putstr_fd("\nreadline error\n", 2);
     	cleanup_line_runtime(d);

    // Now free long-lived containers once at exit
    	vec_free(&d->vec_env);
    	vec_free(&d->vec_cmds);
    	vec_free(&d->vec_tok);

    	arena_free(&d->arena_env);
    	arena_free(&d->arena_tok);
        exit(EXIT_SUCCESS); //should be error and clean exit but EXIT_SUCCESS for testing purpose
    }
    if (*line)
    {
        add_history(line);
        tokenizer(d, line);
		syntax_validation(d);
       // debug_print_tokens(d); //for testing
       // if tok_validation(d) then:
		build_vec_cmds(d);
        executor(d); //leak from here
		cleanup_line_runtime(d);
        arena_reset(&d->arena_tok);
        //vec_reset(&d->vec_tok);
		//vec_reset(&d->vec_cmds);

     }
    free(line);
}

int main(int argc, char **argv, char **envp)
{
    t_data *d;

    (void)argc;
    (void)argv;
    d = get_data();
    shell_init(d, envp);
    while (1) //leak from here
        read_the_line(d);
    cleanup_shell(d);
    return (0);
}
