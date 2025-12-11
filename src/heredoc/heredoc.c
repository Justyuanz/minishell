/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:31 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/11 13:14:56 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

int	heredoc_stuff(t_data *d, t_cmd *cmd, t_shell *shell)
{
	int	hd_ret;

	hd_ret = handle_heredocs(d, cmd);
	if (hd_ret == 1)
		shell->exitcode = 1;
	else if (hd_ret == 130)
	{
		shell->exitcode = 130;
		return (1);
	}
	return (0);
}

int	count_hd(t_cmd *cmd, int here_count)
{
	size_t	i;
	t_redir	*redir;

	i = 0;
	while (i < cmd->redirs.len)
	{
		redir = get_redir(cmd, i);
		if (redir->type == HEREDOC)
			here_count++;
		i++;
	}
	return (here_count);
}

char	*create_heredoc_filename(int heredoc_num)
{
	char	*num_str;
	char	*filename;

	num_str = ft_itoa(heredoc_num);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", num_str);
	free(num_str);
	return (filename);
}

static int	handle_single_heredoc(t_data *d, t_redir *redir, int count)
{
	int		hd_ret;
	char	*filename;
	char	*delim;

	set_heredoc_signal();
	filename = create_heredoc_filename(count);
	if (!filename)
		return (1);
	delim = ft_strdup(redir->file);
	redir->file = ft_strdup(filename);
	redir->strdupped = 88;
	free (filename);
	hd_ret = read_heredoc_input(d, delim, redir->file, redir);
	free(delim);
	set_prompt_signals();
	if (hd_ret == -1)
	{
		perror("heredoc");
		return (1);
	}
	else if (hd_ret == 130)
		return (130);
	return (0);
}

int	handle_heredocs(t_data *d, t_cmd *cmd)
{
	static int	heredoc_count;
	t_redir		*redir;
	size_t		i;
	int			ret;

	i = 0;
	heredoc_count = 0;
	while (i < cmd->redirs.len)
	{
		redir = get_redir(cmd, i);
		redir->strdupped = 0;
		if (redir->type == HEREDOC)
		{
			heredoc_count++;
			ret = handle_single_heredoc(d, redir, heredoc_count);
			if (ret != 0)
				return (ret);
		}
		i++;
	}
	return (0);
}
