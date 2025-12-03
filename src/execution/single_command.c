/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:31:15 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/03 23:32:19 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_wait_status(t_shell *shell, int *nl_flag)
{
	int	status;
	int	signal;

	status = 127;
	waitpid(-1, &status, 0);
	if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		shell->exitcode = signal + 128;
		if (WTERMSIG(status) == SIGINT)
		{
			if (*nl_flag == 0)
			write(1, "\n", 1);
			*nl_flag = 1;
		}
		//if (signal != SIGPIPE)
		//	return ;  //should not return here
	}
	if (WIFEXITED(status))
		update_exitcode(WEXITSTATUS(status), shell);
}

void	wait_for_all(t_shell *shell)
{

	int	i;
	int	nl_flag;
	
	nl_flag = 0;
	i = 0;

	if (!shell->pids)
		return ;
	while (shell->pids[i] != 0)
	{
		if (shell->pids[i] > 0)
			check_wait_status(shell, &nl_flag);
		i++;
	}
}

void	execute_single_command(t_shell *shell)
{
	char	*command_path;
	t_cmd	*cmd;

	cmd = get_cmd(shell->data, 0);
	if (cmd->redirs.len > 0)
		redirect_child(cmd, shell);
	command_path = get_command_path(cmd->argv[0], shell);
	if (command_path)
	{
		if (execve(command_path, cmd->argv, shell->envp) == -1)
		{
			free(command_path);
			command_path = NULL;
		}
	}
	// cleanup_child(shell);
	exit(shell->exitcode);
}

void	handle_single_command(t_data *d, t_cmd *cmd, t_shell *shell)
{
	if (handle_heredocs(d, cmd) != 0)
	{
		shell->exitcode = 1;
	}

	shell->pids[0] = fork();
	if (shell->pids[0] < 0)
		error_smt();
	if (shell->pids[0] == 0)
	{
		set_child_signals();
		execute_single_command(shell);
	}
	else
	{
		set_parent_wait_signals();
		wait_for_all(shell);
		set_prompt_signals();
	}
}

void	single_command_case(t_data *d, t_shell *shell)
{
	int		flag;
	t_cmd	*cmd;
	int		savestdout;
	int		savestdin;
	
	savestdout = dup(STDOUT_FILENO);
	savestdin = dup(STDIN_FILENO);
	cmd = get_cmd(shell->data, 0);
	if (cmd)
	{
		flag = check_if_builtin(shell, cmd->argv[0]);
		if (flag != 0)
		{
			if (cmd->redirs.len > 0)
				redirect_child(cmd, shell);
			handle_builtin(flag, cmd, shell);
		}
		else
			handle_single_command(d, cmd, shell);
	}
	dup2(savestdout, STDOUT_FILENO);
	dup2(savestdin, STDIN_FILENO);
	close(savestdout);
}
