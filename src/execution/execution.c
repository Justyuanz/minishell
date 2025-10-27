#include "minishell.h"

void    handle_execution(t_cmd *cmd, t_shell *shell)
{
    char	*cmd_path;

	if (cmd->redirs.len > 0)
        redirect_child(cmd, shell);
	
	cmd_path = get_command_path(cmd->argv[0], shell);
	if (cmd_path)
	{
		if (execve(cmd_path, cmd->argv, shell->envp) == -1)
		{
			free(cmd_path);
			cmd_path = NULL;
		}
	}
	//cleanup_child(shell);
	exit(shell->exitcode);
}
void close_parent_pipes(t_shell *shell)
{
    if (shell->index > 0 && shell->pipe_array[shell->index - 1])
        close(shell->pipe_array[shell->index - 1][0]);
    if (shell->index < shell->command_index - 1 && shell->pipe_array[shell->index])
        close(shell->pipe_array[shell->index][1]);
}

void    do_command_fork(t_cmd *cmd, t_shell *shell)
{
    int flag_builtin;
    //printf("\n%i\n", shell->index);
    shell->pids[shell->index] = fork();
    if (shell->pids[shell->index ] < 0)
        return ;
    if (shell->pids[shell->index] == 0)
    {
        handle_pipes(shell); 
        flag_builtin = check_if_builtin(cmd->argv[0]);
        if (flag_builtin != 0)
            handle_builtin(flag_builtin, cmd, shell);
        else
            handle_execution(cmd, shell); 
    }
    else
        close_parent_pipes(shell);

}

int	is_empty_or_space(const char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return (1);
	while (*cmd)
	{
		if (*cmd != ' ')
			return (0);
		cmd++;
	}
	return (1);
}

int	check_if_valid_command(t_cmd *cmd, t_shell *shell)
{
	if (is_empty_or_space(cmd->argv[0]))
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 21);
		shell->exitcode = 127;
		//if (shell->data->vec_cmds.len > 1)
		//	close_pipes_in_parent(shell);
		return (1);
	}
	return (0);
}

void    handle_command(t_shell *shell, int command_count)
{
    t_cmd   *cmd;
    shell->index = 0;
    while (shell->index < command_count)
    {
        
        cmd = get_cmd(shell->data, shell->index);
            if (cmd->argv)
            {
                if (!check_if_valid_command(cmd, shell))
                {
                    // if (cmd->redirs.len > 0)
                    // {
                    //     check_for_heredocs(cmd, shell);
                    //     if (g_signal == SIGINT)
                    //         return ;
                    // }
                    do_command_fork(cmd, shell);
                }
            }
        shell->index++;
    }
}


int create_pids(t_shell *shell, int command_count)
{
    int i;

    i = 0;
    shell->pids = NULL;
    shell->pids = malloc(command_count * sizeof(int));
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

void    shell_execution(t_shell *shell)
{
    int     command_count;
   // printf("Here");
    shell->index = 0;
    command_count = shell->data->vec_cmds.len;
    //printf("Here1");
    shell->pipes_count = command_count - 1;
    shell->command_index = command_count;
    //printf("pipe count %i\n", shell->command_index);
    if (create_pids(shell, command_count))
    {
        printf("pids creation failed \n");
        return ;
    }
    if (command_count == 1)
    {
        single_command_case(shell);
    }
    else
    {
        if (create_pipes(shell) == -1) 
            printf("pipe creation failed \n");  
        handle_command(shell, command_count);
        wait_for_all(shell);
        free_pipes(shell);
    }
    //cleanup_parent(shell);
    shell->command_index = 0;
    
}
