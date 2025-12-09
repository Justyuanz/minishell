/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:27:44 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:27:46 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_home(char *old_pwd, t_shell *shell)
{
	char	*home;

	home = get_env_value(shell, "HOME");
	if (!home || home[0] == '\0')
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		free_string(old_pwd);
		return (1);
	}
	if (chdir(home) == -1)
	{
		free_string(old_pwd);
		ft_putstr_fd("chdir syscall failed\n", 2);
		update_exitcode(1, shell);
		return (1);
	}
	return (0);
}

int	update_old_pwd(t_vec *vec_env, char *old_pwd)
{
	t_env	*env_var;
	size_t	i;

	i = 0;
	if (!old_pwd || !vec_env)
		return (1);
	while (i < vec_env->len)
	{
		env_var = (t_env *)vec_get(vec_env, i);
		if (env_var && env_var->key && ft_strcmp("OLDPWD", env_var->key) == 0)
		{
			env_var->value = ft_strdup(old_pwd);
			env_var->value_dupped = 1;
			free(old_pwd);
			old_pwd = NULL;
			return (0);
		}
		i++;
	}
	ft_putstr_fd("OLDPWD not found\n", 2);
	free(old_pwd);
	old_pwd = NULL;
	return (1);
}

void	error_cd(int flag, t_shell *shell)
{
	if (flag == 1)
		ft_putstr_fd("cd: too many arguments\n", 2);
	if (flag == 2)
		ft_putstr_fd("No such file or directory\n", 2);
	update_exitcode(1, shell);
}

void	builtin_cd(int i, char **command_array, t_shell *shell)
{
	char	*old_pwd;

	while (command_array[i])
		i++;
	if (i > 2)
		return (error_cd(1, shell));
	old_pwd = getcwd(NULL, 0);
	if (command_array[1] == NULL || (ft_strcmp(command_array[1], "--") == 0))
	{
		if (cd_home(old_pwd, shell))
		{
			update_exitcode(1, shell);
			return ;
		}
	}
	else
	{
		if (chdir(command_array[1]) == -1)
		{
			free_string(old_pwd);
			return (error_cd(2, shell));
		}
	}
	update_old_pwd(&shell->data->vec_env, old_pwd);
}
