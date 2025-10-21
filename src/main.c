#include "minishell.h"

//updated vec_new vec_push vec_get vec_resize to work with array of ptr

//a func to destroy
//a func to exit?
//tokenizerecho

void read_the_line(t_data *d)
{
    char  *line;

    line = readline("minishell$");
    if (!line)
        exit(EXIT_FAILURE); //error handling
    if (*line)
    {
        add_history(line);
        tokenizer(d, line);
       // debug_print_tokens(d); //for testing
		build_vec_cmds(d);
        executor(d);
        arena_reset(&d->arena_tok);
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
    shell_init(d, envp);
    while (1)
        read_the_line(d);
    arena_free(&d->arena_tok);
    vec_free(&d->vec_tok);
    return 0;
}
