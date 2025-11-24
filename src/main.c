#include "minishell.h"

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
		if (syntax_validation(d))
		{
            build_vec_cmds(d);
            shell->data = d;
            shell->envp = create_envp_from_data(d);
            shell_execution(d, shell);
        }
		cleanup_line_runtime(d);
        arena_reset(&d->arena_tok);
        //vec_reset(&d->vec_tok);
		//vec_reset(&d->vec_cmds) ;

        vec_reset(&d->vec_tok);
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
    cleanup_shell(d);
    return (0);
}
