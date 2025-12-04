/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:51 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:30:53 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_paths(const char *dir, const char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	char	*full_path;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, dir_len + 1);
	full_path[dir_len] = '/';
	ft_strlcpy(full_path + dir_len + 1, cmd, cmd_len + 1);
	return (full_path);
}

char	*search_in_cwd(const char *cmd, t_shell *shell)
{
	char	*cwd;
	char	*cmd_path;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		update_exitcode(1, shell);
		ft_putstr_fd("cwd call failed", 2);
		return (NULL);
	}
	cmd_path = join_paths(cwd, cmd);
	free(cwd);
	cwd = NULL;
	if (cmd_path)
	{
		if (access(cmd_path, X_OK) == 0)
		{
			shell->exitcode = 0;
			return (cmd_path);
		}
		free(cmd_path);
		cmd_path = NULL;
	}
	return (NULL);
}
