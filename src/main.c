/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:18:54 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/06 00:55:18 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_the_line(t_data *d, t_shell *shell)
{
	int i;

	i = 0;
	d->line = readline("minishell$ ");
	if (!d->line)
	{
		while (shell->envp[i])
		{
			free(shell->envp[i]);
			i++;
		}
		free(shell->envp);
		destroy_and_exit(d, NULL, 0);
	}
	if (*d->line)
	{
		add_history(d->line);
		if (tokenizer(d))
		{
			printf("tknzr done\n");
			if (syntax_validation(d))
			{
				printf("syntax check done\n");
				build_vec_cmds(d);
				printf("build cmd done\n");
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
