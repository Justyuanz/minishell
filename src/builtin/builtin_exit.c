/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:28:54 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/07 15:28:51 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	final_exit(t_shell *shell, int exit_number)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		free(shell->envp[i]);
		i++;
	}
	free(shell->envp);
	if (shell->pids)
	{
		free(shell->pids);
		shell->pids = NULL;
	}
	if (shell->command_index > 1)
		free_pipes(shell);
	cleanup_line_runtime(shell->data);
	close(shell->savestdout);
	close(shell->savestdin);
	destroy_and_exit(shell->data, NULL, exit_number);
}

int	is_exit_digit(char *exit_str)
{
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	while (exit_str[i] != '\0')
	{
		if (ft_isdigit(exit_str[i]) == 0)
			ret++;
		i++;
	}
	if (ret > 1)
		return (ret);
	if (ft_strlen(exit_str) > 19)
		return (1);
	if (exit_str[0] == '-')
		return (ft_strcmp(exit_str, "-9223372036854775808") > 0);
	else
	{
		if (exit_str[0] == '+')
			exit_str++;
		return (ft_strcmp(exit_str, "9223372036854775807") > 0);
	}
}

void	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	int	exit_number;

	exit_number = 0;
	ft_putstr_fd("exit\n", 1);
	if (cmd->argv[1] == NULL)
		final_exit(shell, shell->exitcode);
	if (is_exit_digit(cmd->argv[1]))
	{
		exit_number = 2;
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		final_exit(shell, exit_number);
	}
	if (cmd->argv[1] && cmd->argv[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		shell->exitcode = 1;
		return ;
	}
	else if (cmd->argv[1] && exit_number == 0)
		exit_number = ft_atoi(cmd->argv[1]);
	final_exit(shell, exit_number);
}
