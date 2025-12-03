/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:18:54 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/03 17:27:06 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_the_line(t_data *d, t_shell *shell)
{
	d->line = readline("minishell$ ");
	if (!d->line)
	{
		destroy_and_exit(d, NULL, 0);
				//exit(ft_shell()->exitcode);
	}
	if (*d->line)
	{
		//set_prompt_signals();
		add_history(d->line);
		set_prompt_signals();
		if (tokenizer(d))
		{
			//debug_print_tokens(d);
			if (syntax_validation(d))
			{
				build_vec_cmds(d);
				//debug_print_cmds(d);
				shell_execution(d, shell);
			}
		}
		cleanup_line_runtime(d);
	}
	free(d->line);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*d;
	t_shell	*shell;

	(void)argc;
	(void)argv;
	d = get_data();
	shell = ft_shell();
	shell_init(d, shell, envp);
	//fprintf(stderr,"calling sigint in main\n");
	while (1)
		read_the_line(d, shell);
	cleanup_shell(d);
	return (0);
}
