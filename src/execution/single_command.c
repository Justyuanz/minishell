#include "minishell.h"

void    wait_for_all(t_shell *shell)
{
    int status;
    int i;
    int signal;

    i = 0;
    status = 127;
    if (!shell->pids)
        return ;
    while (shell->pids[i] != 0)
    {
        if (shell->pids[i] > 0)
        {
            waitpid(shell->pids[i], &status, 0);
            if (WIFSIGNALED(status))
            {
                signal = WTERMSIG(status);
                shell->exitcode = signal + 128;
                if (signal != 13)
                    return ;
            }
            if (WIFEXITED(status))
                update_exitcode(WEXITSTATUS(status), shell);
        }
        i++;
    }
}

void    execute_single_command(t_shell *shell)
{
    char    *command_path;
    t_cmd   *cmd;

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
    //cleanup_child(shell);
    exit(shell->exitcode);
}

void    handle_single_command(t_data *d, t_cmd *cmd, t_shell *shell)
{
    if (handle_heredocs(d, cmd) != 0)
    {
        shell->exitcode = 1;
    }
    shell->pids[0] = fork();
    if (shell->pids[0] < 0)
        error_smt();
    if (shell->pids[0] == 0)
        execute_single_command(shell);
    else
        wait_for_all(shell);
}


void    single_command_case(t_data *d, t_shell *shell)
{
    int flag;
    t_cmd   *cmd;
    t_redir	*redir;
	int savestdout =  dup(STDOUT_FILENO);
	int savestdin = dup(STDIN_FILENO);
	int fd;
	size_t	i = 0;

	/*
    if (!shell) {
        printf("ERROR: shell is NULL\n");
        return;
    }

    if (!shell->data) {
        printf("ERROR: shell->data is NULL\n");
        return;
    }
    */
    cmd = get_cmd(shell->data, 0);

    if (cmd)
    {
		while (i < cmd->redirs.len)
		{
			redir = get_redir(cmd, i);
			if (redir->type == REDIR_OUT)
			{
				fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				// if (fd = -1)
				dup2(fd, STDOUT_FILENO);
				// if(dup2) == -1
				close(fd);
			}
			else if (redir->type == APPEND)
			{
				fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
				// if (fd = -1)
				dup2(fd, STDOUT_FILENO);
				// if(dup2) == -1
				close(fd);
			}
			else if (redir->type == REDIR_IN)
			{
				fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
				// if (fd = -1)
				dup2(fd, STDIN_FILENO);
				// if(dup2) == -1
				close(fd);
			}
			i++;
		}
        flag = check_if_builtin(shell, cmd->argv[0]);
        if (flag != 0)
            handle_builtin(flag, cmd, shell);
        else // should i check here for valid command or not??
            handle_single_command(d, cmd, shell);
    }
	dup2(savestdout, STDOUT_FILENO);
	dup2(savestdin, STDIN_FILENO);
	close(savestdout);
}
