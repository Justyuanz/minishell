#include "minishell.h"
// Fix t_cmd to own t_vec argv
//Remove vec_new(&d->vec_cmds, ...) from init_cmd
//In shell_init, use sizeof(t_cmd *) for vec_cmds
//Add vec_free(&cmd->argv) in cleanup_line
//??Call cleanup_line(d) before arena_reset(&d->arena_tok)
//Keep your arena alignment fix
void read_the_line(t_data *d)
{
    char  *line;

    line = readline("minishell$");
    if (!line)
    {
        //cleanup_shell(d);
        ft_putstr_fd("\nreadline error\n", 2);
        free(line); //delete me
        cleanup_shell(d); //delete me
        exit(EXIT_SUCCESS); //should be error but EXIT_SUCCESS for testing purpose
    }
    if (*line)
    {
        add_history(line);
        tokenizer(d, line);
       // debug_print_tokens(d); //for testing
       // if tok_validation(d) then:
		build_vec_cmds(d);
        executor(d); //leak from here
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
    while (1) //leak from here
        read_the_line(d);
    cleanup_shell(d);
    return 0;
}
