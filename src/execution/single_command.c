/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:31:15 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/03 18:28:20 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_all(t_shell *shell)
{
	int	status;
	int	i;
	int	signal;
	int	nl_flag;

	i = 0;
	nl_flag = 0;
	status = 127;
	if (!shell->pids)
		return ;
	while (shell->pids[i] != 0)
	{
		if (shell->pids[i] > 0)
		{
			waitpid(-1, &status, 0);
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    		{
				if (nl_flag == 0)
					write(1, "\n", 1);
				nl_flag = 1;
			}
			if (WIFSIGNALED(status))
			{
				signal = WTERMSIG(status);
				shell->exitcode = signal + 128;
				//if (signal != SIGPIPE)
				//	return ;  //should not return here
			}
			if (WIFEXITED(status))
				update_exitcode(WEXITSTATUS(status), shell);
		}
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
		//fprintf(stderr,"calling signal in child process, pid:%d\n", shell->pids[0]);
		set_child_signals();
		execute_single_command(shell);
	}
	else
	{
		//fprintf(stderr,"calling wait signal in parent process, pid:%d\n", shell->pids[0]);
		set_parent_wait_signals();
		wait_for_all(shell);
		//fprintf(stderr,"calling prompt signal in parent process, pid:%d\n", shell->pids[0]);
		set_prompt_signals();
	}
}

void	single_command_case(t_data *d, t_shell *shell)
{
	int		flag;
	t_cmd	*cmd;
	//t_redir	*redir;
	int		savestdout;
	int		savestdin;
	//int		fd;
	//size_t	i;

	savestdout = dup(STDOUT_FILENO);
	savestdin = dup(STDIN_FILENO);
	//i = 0;
	/*
	if (!shell) {
		printf("ERROR: shell is NULL\n");
		return ;
	}
	if (!shell->data) {
		printf("ERROR: shell->data is NULL\n");
		return ;
	}
	*/
	cmd = get_cmd(shell->data, 0);
	if (cmd)
	{
		//redirect_child(shell->)
		// while (i < cmd->redirs.len)
		// {
		// 	redir = get_redir(cmd, i);
		// 	if (redir->type == REDIR_OUT)
		// 	{
		// 		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		// 		// if (fd = -1)
		// 		dup2(fd, STDOUT_FILENO);
		// 		// if(dup2) == -1
		// 		close(fd);
		// 	}
		// 	else if (redir->type == APPEND)
		// 	{
		// 		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		// 		// if (fd = -1)
		// 		dup2(fd, STDOUT_FILENO);
		// 		// if(dup2) == -1
		// 		close(fd);
		// 	}
		// 	else if (redir->type == REDIR_IN)
		// 	{
		// 		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		// 		// if (fd = -1)
		// 		dup2(fd, STDIN_FILENO);
		// 		// if(dup2) == -1
		// 		close(fd);
		// 	}
		// 	i++;
		// }
		flag = check_if_builtin(shell, cmd->argv[0]);
		if (flag != 0)
		{
			if (cmd->redirs.len > 0)
				redirect_child(cmd, shell);
			handle_builtin(flag, cmd, shell);
		}
		else // should i check here for valid command or not??
			handle_single_command(d, cmd, shell);
	}
	dup2(savestdout, STDOUT_FILENO);
	dup2(savestdin, STDIN_FILENO);
	close(savestdout);
}
