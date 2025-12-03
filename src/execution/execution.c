/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:20 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/02 17:00:32 by jinzhang         ###   ########.fr       */
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
		//fprintf(stderr,"calling signal in child process, pid:%d\n", shell->pids[shell->index]);
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
		//fprintf(stderr,"calling wait signal in parent process, pid:%d\n", shell->pids[shell->index]);
		set_parent_wait_signals();
		close_parent_pipes(shell);
	}
}

void	handle_command(t_data *d, t_shell *shell, int command_count)
{
	t_cmd	*cmd;

	shell->index = 0;
	while (shell->index < command_count)
	{
		cmd = get_cmd(shell->data, shell->index);
		if (cmd->argv)
		{
			if (handle_heredocs(d, cmd) != 0)
			{
				shell->exitcode = 1;
				shell->index++;
				continue ;
			}
			do_command_fork(cmd, shell);
		}
		shell->index++;
	}
}

int	create_pids(t_shell *shell, int command_count)
{
	int	i;

	i = 0;
	shell->pids = NULL;
	shell->pids = malloc((command_count + 1) * sizeof(int)); // cmd_count + 1
	if (!shell->pids)
		return (1);
	while (i < command_count)
	{
		shell->pids[i] = -1;
		i++;
	}
	shell->pids[command_count] = 0; // out of bound
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
		handle_command(d, shell, command_count);
		wait_for_all(shell);
		//fprintf(stderr,"calling prompt signal in parent process\n");
		//set_prompt_signals();
		free_pipes(shell);
	}
	// cleanup_parent(shell);
	shell->command_index = 0;
}
