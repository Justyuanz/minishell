/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 19:48:42 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/09 19:35:37 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc_line(t_data *d, t_redir *redir, int fd)
{
	char	buf[1024];
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (d->line[i] && j < 1024)
	{
		if (expand_in_heredoc(redir) && d->line[i] == '$')
		{
			handle_expansion(d, buf, &i, &j);
			if (j >= sizeof(buf) - 1)
				break ;
		}
		else
			buf[j++] = d->line[i++];
	}
	buf[j] = '\0';
	write(fd, buf, ft_strlen(buf));
	write(fd, "\n", 1);
}

int	read_heredoc_input(t_data *d, const char *delimiter, char *filename,
		t_redir *redir)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	while (1)
	{
		d->line = readline("> ");
		if (g_signal == SIGINT)
		{
			rl_done = 0;
			g_signal = 0;
			close(fd);
			unlink(filename);
			return (130);
		}
		if (!d->line || ft_strcmp(d->line, delimiter) == 0)
			break ;
		write_heredoc_line(d, redir, fd);
	}
	g_signal = 0;
	rl_done = 0;
	close(fd);
	return (0);
}
