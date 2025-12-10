/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:20 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/10 15:54:29 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static bool	execute_pipeline(t_shell *shell, int command_count)
{
	if (create_pipes(shell) == -1)
	{
		update_exitcode(1, shell);
		ft_putstr_fd("pipe arr call failed\n", 2);
		return (false);
	}
	if (handle_command(shell, command_count) == 130)
	{
		wait_for_all(shell);
		shell->exitcode = 130;
		free_pipes(shell);
		return (false);
	}
	wait_for_all(shell);
	free_pipes(shell);
	return (true);
}

void	cleanup_parent(t_shell *shell)
{
	if (shell->pids)
	{
		free(shell->pids);
		shell->pids = NULL;
	}
}

bool	handle_exec_here(t_shell *shell, t_data *d)
{
	t_cmd *cmd;
	size_t i;

	i = 0;
	while (i < d->vec_cmds.len)
	{
		cmd = get_cmd(d, i);
		if (heredoc_stuff(d, cmd, shell) != 0)
			return(false) ;
		i++;
	}
	return (true);
}

void	shell_execution(t_data *d, t_shell *shell)
{
	int	command_count;

	shell->index = 0;
	shell->is_amb = false;
	command_count = shell->data->vec_cmds.len;
	shell->pipes_count = command_count - 1;
	shell->command_index = command_count;
	if (handle_exec_here(shell, d))
	{
		if (create_pids(shell, command_count))
		{
			update_exitcode(1, shell);
			ft_putstr_fd("pids creation failed \n", 2);
			return ;
		}
		if (command_count == 1)
			single_command_case(shell);
		else
		{
			if (!execute_pipeline(shell, command_count))
				return ;
		}
	}
	cleanup_parent(shell);
	shell->command_index = 0;
}
