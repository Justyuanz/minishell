/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:31 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 19:20:15 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

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



int	handle_heredocs(t_data *d, t_cmd *cmd)
{
	t_redir		*redir;
	char		*filename;
	char		*delim;
	size_t		i;
	static int	heredoc_count;
	int			hd_ret;

	heredoc_count = 0;
	i = 0;
	while (i < cmd->redirs.len)
	{
		redir = get_redir(cmd, i);
		if (redir->type == HEREDOC)
		{
			set_heredoc_signal();
			heredoc_count++;
			filename = create_heredoc_filename(heredoc_count);
			if (!filename)
				return (1);
			delim = ft_strdup(redir->file);
			redir->file = filename;
			hd_ret = read_heredoc_input(d, delim, redir->file, redir);
			free(delim);
			set_prompt_signals();
			if (hd_ret == -1)
			{
				perror("heredoc");
				return (1);
			}
			else if (hd_ret == 130)
			{
				return (130);
			}
		}
		i++;
	}
	return (0);
}
