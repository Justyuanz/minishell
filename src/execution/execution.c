#include "minishell.h"
/*
void    shell_init(char **envp, t_shell *shell)
{
    shell->exitcode = 0;
    shell->index = 0;
    shell->command_index = 0;
    shell->input = STDIN_FILENO;
    shell->output = STDOUT_FILENO;
}
*/
void    handle_execution(t_cmd *command, t_shell *shell)
{
    char    *command_path;

    command_path = get_command_path(command->argv[0], shell);
    if (command_path)
    {
        if (execve(command_path, command->argv, shell->envp) == -1)
        {
            free(command_path);
            command_path = NULL;
        }
    }
    exit(shell->exitcode);
}
void	close_parent_pipes(t_shell *shell)
{
	if (shell->index > 1)
		close(shell->pipe_array[shell->index - 2][0]);
	if (shell->index < shell->command_index)
		close(shell->pipe_array[shell->index - 1][1]);
}

void    do_command_fork(t_cmd *command, t_shell *shell)
{
    int flag_builtin;
    printf("\n%i\n", shell->index);
    shell->pids[shell->index - 1] = fork();
    if (shell->pids[shell->index - 1] < 0)
        return ;
    if (shell->pids[shell->index - 1] == 0)
    {
        handle_pipes(shell); 
        flag_builtin = check_if_builtin(command->argv[0]);
        if (flag_builtin != 0)
            handle_builtin(flag_builtin, command, shell);
        else
            handle_execution(command, shell); 
    }
    else
        close_parent_pipes(shell);
}

void    handle_command(t_shell *shell, int command_count)
{
    shell->index = 0;
    t_cmd   *cmd;
    while (shell->index < command_count)
    {
        cmd = get_cmd(shell->data, shell->index);
        if (cmd->argv)
        {   
            //if (check_valid_command(shell, cmd->argv[shell->index]))
            do_command_fork(cmd, shell);
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

    command_count = shell->data->vec_cmds.len;
    //printf("Here1");
    shell->pipes_count = command_count - 1;
    shell->command_index = command_count;
    //printf("pipe count %i\n", shell->command_index);
    if (create_pids(shell, command_count))
    {
        error_smt();
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
    shell->index = 0;
}
