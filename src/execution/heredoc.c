/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:31 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/03 16:33:56 by jinzhang         ###   ########.fr       */
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

// char	*expanded_line_heredoc(t_data *d)
// {
// 	char	tmp[1024];
// 	size_t	i;
// 	size_t	j;

// 	i = 0;
// 	j = 0;
// 	while (d->line[i])
// 	{
// 		if (d->line[i] == '$')
// 			handle_expansion(d, tmp, &i, &j);
// 		tmp[j++] = d->line[i++];
// 	}
// 	tmp[j] = '\0';
// 	return (ft_strdup(tmp));
// }

int	read_heredoc_input(t_data *d, const char *delimiter, const char *filename,
		t_redir *redir)
{
	int		fd;
	//char	*expanded;
	char	buf[1024];
	size_t	j;
	size_t	i;

	i = 0;
	j = 0;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	while (1)
	{
		d->line = readline("> ");
		if (!d->line || ft_strcmp(d->line, delimiter) == 0)
		{
			//free
			break ;
		}
		if (expand_in_heredoc(redir))
		{
			while (d->line[i])
			{
				if (d->line[i] == '$')
					handle_expansion(d, buf, &i, &j);
				buf[j++] = d->line[i++];
			}
			buf[j] = '\0';
			//write(fd, expanded, ft_strlen(expanded));
			//write(fd, "\n", 1);
			//free(expanded);
		}
		else
		{
			while (d->line[i])
			{
				buf[j++] = d->line[i++];
			}
			buf[j] = '\0';
			//write(fd, d->line, ft_strlen(d->line));
			//write(fd, "\n", 1);
		}
		if (g_signal == SIGINT)
		{
			rl_done = 0;
			close (fd);
			return (130);
		}
		free(d->line);
	}
	write(fd, buf, ft_strlen(buf));
	write(fd, "\n", 1);
	rl_done = 0;
	close(fd);
	return (0);
}

int	handle_heredocs(t_data *d, t_cmd *cmd)
{
	t_redir		*redir;
	char		*filename;
	char		*delim;
	size_t		i;
	static int	heredoc_count;

	heredoc_count = 0;
	i = 0;
	while (i < cmd->redirs.len)
	{
		redir = get_redir(cmd, i);
		if (redir->type == HEREDOC)
		{
			//fprintf(stderr,"calling signal in handle heredoc \n");
			set_heredoc_signal();
			heredoc_count++;
			filename = create_heredoc_filename(heredoc_count);
			if (!filename)
				return (1);
			delim = ft_strdup(redir->file);
			redir->file = filename;
			if (read_heredoc_input(d, delim, redir->file, redir) == -1)
			{
				perror("heredoc");
				return (1);
			}
			set_prompt_signals();
			free(delim);
		}
		i++;
	}
	return (0);
}
