/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:27:25 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:27:27 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_builtin(t_shell *shell, char *command)
{
	if (!command)
	{
		shell->exitcode = 0;
		return (0);
	}
	if (ft_strcmp(command, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp(command, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(command, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(command, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp(command, "unset") == 0)
		return (BUILTIN_UNSET);
	if (ft_strcmp(command, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(command, "exit") == 0)
		return (BUILTIN_EXIT);
	else
		return (0);
}

void	handle_builtin(int flag, t_cmd *command, t_shell *shell)
{
	if (flag == BUILTIN_CD)
		builtin_cd(0, command->argv, shell);
	if (flag == BUILTIN_ECHO)
		builtin_echo(command, shell);
	if (flag == BUILTIN_PWD)
		builtin_pwd(shell);
	if (flag == BUILTIN_EXPORT)
		builtin_export(command, shell);
	if (flag == BUILTIN_UNSET)
		builtin_unset(command, shell);
	if (flag == BUILTIN_ENV)
		builtin_env(shell);
	if (flag == BUILTIN_EXIT)
		builtin_exit(command, shell);
}
