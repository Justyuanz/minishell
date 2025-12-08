/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:18:54 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/08 11:31:42 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_the_line(t_data *d, t_shell *shell)
{
	// if (isatty(fileno(stdin)))
	// 	d->line = readline("mini>>");
	// else
	// {
	// 	char *line;
	// 	line = get_next_line(fileno(stdin));
	// 	d->line = ft_strtrim(line, "\n");
	// 	free(line);
	// }

	d->line = readline("minishell$ ");
	if (!d->line)
		eof_cleanup(d, shell);
	if (*d->line)
	{
		add_history(d->line);
		if (tokenizer(d))
		{
			if (syntax_validation(d))
			{
				build_vec_cmds(d);
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
	set_prompt_signals();
	while (1)
		read_the_line(d, shell);
	cleanup_shell(d);
	return (0);
}
