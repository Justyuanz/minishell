/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:31:15 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/09 11:29:12 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_wait_status(t_shell *shell, int *nl_flag)
{
	int	status;

	status = 127;
	waitpid(-1, &status, 0);
	if (WIFSIGNALED(status))
	{
		shell->exitcode = 0;
		if (WTERMSIG(status) == SIGINT)
		{
			if (*nl_flag == 0)
				write(1, "\n", 1);
			*nl_flag = 1;
		}
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
	if (!cmd || cmd->argv[0] == NULL || cmd->argv[0][0] == '\0')
	{
		//printf("here here\n");
		//exit(shell->exitcode);
		final_exit(shell, shell->exitcode);
	}
	if (cmd->redirs.len > 0)
		redirect_child(cmd, shell);
	command_path = get_command_path(cmd->argv[0], shell);
	//printf("commad path %s\n", command_path);
	if (command_path)
	{
		if (execve(command_path, cmd->argv, shell->envp) == -1)
		{
			ft_putstr_fd("EXEC failed\n", 2);
			shell->exitcode = 126;
			free(command_path);
			command_path = NULL;
		}
	}
	//printf("here here1\n");
	final_exit(shell, shell->exitcode);
	//exit(shell->exitcode);
}

int	handle_single_command(t_shell *shell)
{
	shell->pids[0] = fork();
	if (shell->pids[0] < 0)
	{
		update_exitcode(1, shell);
		ft_putstr_fd("fork call failed\n", 2);
	}
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
	return (0);
}

void	single_command_case(t_shell *shell)
{
	int		flag;
	t_cmd	*cmd;

	shell->savestdin = dup(STDIN_FILENO);
	cmd = get_cmd(shell->data, 0);
	if (cmd)
	{
		flag = check_if_builtin(shell, cmd->argv[0]);
		if (flag != 0)
		{
			if (cmd->redirs.len > 0)
				redirect_child(cmd, shell);
			if (shell->is_amb == true)
				return ;
			handle_builtin(flag, cmd, shell);
			dup2(shell->savestdin, STDIN_FILENO);
			close(shell->savestdin);
		}
		else
			handle_single_command(shell);	
	}
	dup2(shell->savestdout, STDOUT_FILENO);
	if (shell->savestdin != -1)
	{
		dup2(shell->savestdin, STDIN_FILENO);
		close(shell->savestdin);
	}
	close(shell->savestdout);
}
