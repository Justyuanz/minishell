#include "minishell.h"

int	redir_in(t_redir *redir, t_shell *shell)
{
	if (redir->type == REDIR_IN)
		shell->input = open(redir->file, O_RDONLY);
	else if (redir->type == HEREDOC)
		shell->input = open("here_doc", O_RDONLY);
	if (shell->input < 0)
		return (1);
	if (dup2(shell->input, STDIN_FILENO) == -1)
	{
		close(shell->input);
		return (1);
	}
	close (shell->input);
	return (0);
}
int	redir_out(t_redir *redir, t_shell *shell)
{
	if (redir->type == REDIR_OUT)
		shell->output = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == APPEND)
		shell->output = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (shell->input < 0)
		return (1);
	if (dup2(shell->output, STDOUT_FILENO) == -1)
	{
		close(shell->output);
		return (1);
	}
	close (shell->output);
	return (0);
}

void	redirect_child(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;
	size_t	i;

	i = 0;
	while (i < cmd->redirs.len)
	{
		redir = get_redir(cmd, i);
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			if (redir_in(redir, shell))
			{
				printf("file error\n");
				exit(EXIT_FAILURE);
			}
		}
		if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if(redir_out(redir, shell))
			{
				printf("file out error\n");
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}
}

