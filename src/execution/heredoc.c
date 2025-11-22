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

int	read_heredoc_input(const char *delimiter, const char *filename)
{
	int		fd;
	char	*line;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);

	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (0);
}

int	handle_heredocs(t_cmd *cmd)
{
	t_redir	*redir;
	char	*filename;
    char *delim;
	size_t	i;
	static int	heredoc_count;
    
    heredoc_count = 0;
	i = 0;
	while (i < cmd->redirs.len)
	{
		redir = get_redir(cmd, i);
		if (redir->type == HEREDOC)
		{
			heredoc_count++;
			filename = create_heredoc_filename(heredoc_count);
			if (!filename)
				return (1);
            delim = ft_strdup(redir->file);
			redir->file = filename;
			if (read_heredoc_input(delim, redir->file) == -1)
			{
				perror("heredoc");
				return (1);
			}
            free(delim);
			// if (expand_in_heredoc(redir) == true)
				//expand_heredoc_file -> can use handle_expansion()?
		}
		i++;
	}
	return (0);
}
