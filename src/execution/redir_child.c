/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 17:22:04 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/10 17:22:09 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	err_redirin_heredoc(t_shell *shell, t_redir *redir)
{
	ft_putstr_fd((char *)redir->file, 2);
	ft_putendl_fd(" :permission denied", 2);
	if (shell->single_builtin == 1)
		return (1);
	else
		final_exit(shell, EXIT_FAILURE);
	return (0);
}

static int	err_redirout_append(t_shell *shell, t_redir *redir)
{
	ft_putstr_fd((char *)redir->file, 2);
	ft_putendl_fd(" :permission denied", 2);
	if (shell->single_builtin == 1)
		return (1);
	else
		final_exit(shell, EXIT_FAILURE);
	return (0);
}

static int	err_ambiguous(t_shell *shell)
{
	ft_putstr_fd("ambiguous redirection\n", 2);
	shell->is_amb = true;
	update_exitcode(1, shell);
	return (1);
}

int	redirect_child(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;
	size_t	i;

	i = -1;
	while (++i < cmd->redirs.len)
	{
		redir = get_redir(cmd, i);
		if (redir->is_ambiguous == true && redir->type != HEREDOC)
			return (err_ambiguous(shell));
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			if (redir_in(redir, shell))
				return (err_redirin_heredoc(shell, redir));
		if (redir->type == REDIR_OUT || redir->type == APPEND)
			if (redir_out(redir, shell))
				return (err_redirout_append(shell, redir));
	}
	return (0);
}
