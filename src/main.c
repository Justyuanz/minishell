#include "minishell.h"

//updated vec_new vec_push vec_get vec_resize to work with array of ptr

//a func to destroy
//a func to exit?
//tokenizerecho
t_shell *ft_shell(void)
{
	static t_shell shell;

	return (&shell);
}

void read_the_line(t_data *d, t_shell *shell)
{
    char  *line;
    line = readline("minishell$ ");
    if (!line)
        exit(EXIT_FAILURE); //error handling
    if (*line)
    {
        add_history(line);
        tokenizer(d, line);
       // debug_print_tokens(d); //for testing
		build_vec_cmds(d);
        executor(d);
        shell->data = d;
        shell->envp = create_envp_from_data(d);
        shell_execution(shell);
        //read_env_example(shell->data);
        arena_reset(&d->arena_tok);
        vec_reset(&d->vec_tok);
        shell->exitcode = 0;
     }
    free(line);
}


int main(int argc, char **argv, char **envp)
{
    t_data *d;

    (void)argc;
    (void)argv;
    d = get_data();
    t_shell *shell = ft_shell();
    shell_init(d, envp);
    while (1)
        read_the_line(d, shell);
    arena_free(&d->arena_tok);
    vec_free(&d->vec_tok);
    return 0;
}
