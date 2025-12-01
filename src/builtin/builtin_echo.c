/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:28:40 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:28:45 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// need to think about possible errors here

void	print_echo(char **array, int i)
{
	while (array[i] != NULL)
	{
		write(1, array[i], ft_strlen(array[i]));
		if (array[++i] != NULL)
			write(1, " ", 1);
	}
}

void	builtin_echo(t_cmd *command, t_shell *shell)
{
	int	new_line;
	int	index;
	int	i;

	new_line = 0;
	index = 1;
	if (command->argv[1] != NULL && ft_strcmp(command->argv[1], "-n") == 0)
		index++;
	else
		new_line = 1;
	while (command->argv[index] != NULL && ft_strcmp(command->argv[index],
			"-n") == 0)
		index++;
	i = index;
	print_echo(command->argv, i);
	if (new_line)
		write(1, "\n", 1);
	update_exitcode(0, shell);
}
