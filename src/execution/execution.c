/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:20 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 00:47:15 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_execution(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	cmd_path = get_command_path(cmd->argv[0], shell);
	if (cmd_path)
	{
		if (execve(cmd_path, cmd->argv, shell->envp) == -1)
		{
			free(cmd_path);
			cmd_path = NULL;
		}
	}
	// cleanup_child(shell);
	exit(shell->exitcode);
}

void	close_parent_pipes(t_shell *shell)
{
	if (shell->index > 0 && shell->pipe_array[shell->index - 1])
		close(shell->pipe_array[shell->index - 1][0]);
	if (shell->index < shell->command_index - 1
		&& shell->pipe_array[shell->index])
		close(shell->pipe_array[shell->index][1]);
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
		if (cmd->redirs.len > 0) // moved this from handle_execution
			redirect_child(cmd, shell);
		flag_builtin = check_if_builtin(shell, cmd->argv[0]);
		if (flag_builtin != 0)
		{
			handle_builtin(flag_builtin, cmd, shell);
			exit(shell->exitcode);
		}
		else
			handle_execution(cmd, shell);
	}
	else
	{
		set_parent_wait_signals();
		close_parent_pipes(shell);
	}
}

int	handle_command(t_data *d, t_shell *shell, int command_count)
{
	t_cmd	*cmd;
	int		hd_ret;
	shell->index = 0;
	while (shell->index < command_count)
	{
		cmd = get_cmd(shell->data, shell->index);
		if (cmd->argv)
		{
			hd_ret = handle_heredocs(d, cmd);
			if ( hd_ret== 1)
			{
				shell->exitcode = 1;
				shell->index++;
				continue ;
			}
			else if (hd_ret == 130)
				return 130;
			do_command_fork(cmd, shell);
		}
		shell->index++;
	}
	return (0);
}

int	create_pids(t_shell *shell, int command_count)
{
	int	i;

	i = 0;
	shell->pids = NULL;
	shell->pids = malloc((command_count + 1) * sizeof(int));
	if (!shell->pids)
		return (1);
	while (i < command_count)
	{
		shell->pids[i] = -1;
		i++;
	}
	shell->pids[command_count] = 0;
	return (0);
}

void	shell_execution(t_data *d, t_shell *shell)
{
	int	command_count;

	shell->index = 0;
	command_count = shell->data->vec_cmds.len;
	shell->pipes_count = command_count - 1;
	shell->command_index = command_count;
	if (create_pids(shell, command_count))
	{
		printf("pids creation failed \n");
		return ;
	}
	if (command_count == 1)
	{
		single_command_case(d, shell);
	}
	else
	{
		if (create_pipes(shell) == -1)
			printf("pipe creation failed \n");
		if (handle_command(d, shell, command_count) == 130)
		{
			shell->exitcode = 130;
			free_pipes(shell);
			return;
		}
		wait_for_all(shell);
	}
	// cleanup_parent(shell);
	shell->command_index = 0;
}
