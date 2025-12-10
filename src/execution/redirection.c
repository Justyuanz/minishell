/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:31:06 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/10 17:21:31 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_in(t_redir *redir, t_shell *shell)
{
	if (redir->type == REDIR_IN)
		shell->input = open(redir->file, O_RDONLY);
	else if (redir->type == HEREDOC)
	{
		shell->input = open(redir->file, O_RDONLY);
		free(redir->file);
		redir->file = NULL;
	}
	if (shell->input < 0)
		return (1);
	if (dup2(shell->input, STDIN_FILENO) == -1)
	{
		close(shell->input);
		return (1);
	}
	close(shell->input);
	return (0);
}

int	redir_out(t_redir *redir, t_shell *shell)
{
	if (redir->type == REDIR_OUT)
		shell->output = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == APPEND)
		shell->output = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (shell->output < 0)
		return (1);
	if (dup2(shell->output, STDOUT_FILENO) == -1)
	{
		close(shell->output);
		return (1);
	}
	close(shell->output);
	return (0);
}
