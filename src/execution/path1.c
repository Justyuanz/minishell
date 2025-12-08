/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:40 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 20:33:42 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_full_path(const char *cmd, const char *dir, int *found)
{
	char	*full_path;

	full_path = join_paths(dir, cmd);
	if (!full_path)
		return (NULL);
	if (check_access(full_path, found) == 1)
		return (full_path);
	free(full_path);
	full_path = NULL;
	return (NULL);
}

static char	*search_in_path(const char *cmd, char **dirs, t_shell *shell)
{
	int		i;
	int		found;
	char	*result;

	i = 0;
	found = 0;
	while (dirs[i] != NULL)
	{
		result = check_full_path(cmd, dirs[i], &found);
		if (result)
		{
			free_array(dirs);
			return (result);
		}
		i++;
	}
	free_array(dirs);
	handle_path_error(cmd, shell, found);
	return (NULL);
}

static char	*check_command(const char *cmd, t_shell *shell)
{
	struct stat	path_stat;

	if (stat(cmd, &path_stat) != 0)
	{
		update_exitcode(127, shell);
		ft_putstr_fd((char *)cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		final_exit(shell, shell->exitcode);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd((char *)cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		update_exitcode(126, shell);
		return (NULL);
	}
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (access(cmd, F_OK) == 0)
	{
		update_exitcode(126, shell);
		ft_putstr_fd((char *)cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	return (NULL);
}

char	*get_command_path(const char *cmd, t_shell *shell)
{
	char	*env_path;
	char	*cmd_path;
	char	**directories;

	if (ft_strchr(cmd, '/'))
		return (check_command(cmd, shell));
	env_path = get_env_value(shell, "PATH");
	if (!env_path)
	{
		cmd_path = search_in_cwd(cmd, shell);
		if (!cmd_path)
		{
			ft_putstr_fd((char *)cmd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			final_exit(shell, 127);
		}
		else
			return (cmd_path);
	}
	directories = ft_split(env_path, ':');
	if (!directories)
		return (NULL);
	return (search_in_path(cmd, directories, shell));
}
