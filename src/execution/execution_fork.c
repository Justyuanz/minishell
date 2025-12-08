/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 20:28:18 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 20:28:20 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_execution(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	if (!cmd || cmd->argv[0] == NULL)
		final_exit(shell, shell->exitcode);
	cmd_path = get_command_path(cmd->argv[0], shell);
	if (cmd_path)
	{
		if (execve(cmd_path, cmd->argv, shell->envp) == -1)
		{
			free(cmd_path);
			cmd_path = NULL;
		}
	}
	final_exit(shell, shell->exitcode);
	//exit(shell->exitcode);
}

static void	fork_else(t_shell *shell)
{
	set_parent_wait_signals();
	close_parent_pipes(shell);
}

void	do_command_fork(t_cmd *cmd, t_shell *shell)
{
	int	flag_builtin;

	shell->pids[shell->index] = fork();
	if (shell->pids[shell->index] < 0)
		return ;
	if (shell->pids[shell->index] == 0)
	{
		set_child_signals();
		handle_pipes(shell);
		if (cmd->redirs.len > 0)
			redirect_child(cmd, shell);
		if (shell->is_amb == true)
			exit(shell->exitcode);
		flag_builtin = check_if_builtin(shell, cmd->argv[0]);
		if (flag_builtin != 0)
		{
			handle_builtin(flag_builtin, cmd, shell);
			final_exit(shell, shell->exitcode);
		}
		else
			handle_execution(cmd, shell);
	}
	else
		fork_else(shell);
}

int	handle_command(t_data *d, t_shell *shell, int command_count)
{
	t_cmd	*cmd;
	int		hd_ret;

	shell->index = 0;
	while (shell->index < command_count)
	{
		shell->is_amb = false;
		cmd = get_cmd(shell->data, shell->index);
		if (cmd->argv)
		{
			hd_ret = handle_heredocs(d, cmd);
			if (hd_ret == 1)
			{
				shell->exitcode = 1;
				shell->index++;
				continue ;
			}
			else if (hd_ret == 130)
				return (130);
			do_command_fork(cmd, shell);
		}
		shell->index++;
	}
	return (0);
}
