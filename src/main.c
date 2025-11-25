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
		destroy_and_exit(d, "exit", 0);
    if (*line)
    {
        add_history(line);
        if (tokenizer(
			d, line))
		{
       		// debug_print_tokens(d);
			if (syntax_validation(d))
			{
            	build_vec_cmds(d);
            //debug_print_cmds(d);
            	shell_execution(d, shell);
        	}
		}
		cleanup_line_runtime(d);
     }
    free(line);
}


int main(int argc, char **argv, char **envp)
{
    t_data *d;
	t_shell *shell;

    (void)argc;
    (void)argv;
    d = get_data();
    shell = ft_shell();
    shell_init(d, shell, envp);
    while (1)
        read_the_line(d, shell);
    cleanup_shell(d);
    return (0);
}
