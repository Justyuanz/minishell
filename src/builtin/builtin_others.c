/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:29:15 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:29:18 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_pwd(t_shell *shell)
{
	char	cwd[1000000];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_putstr_fd("pwd: error retrieving current directory", 2);
		update_exitcode(1, shell);
		return ;
	}
	printf("%s\n", cwd);
	update_exitcode(0, shell);
}

void	builtin_env(t_shell *shell)
{
	t_env	*env;
	size_t	i;
	t_cmd *cmd;

	i = 0;
	cmd = get_cmd(shell->data, shell->index);
	if (cmd->argv[1] != NULL)
	{
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd((char *)cmd->argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		shell->exitcode = 127;
	}
	else
	{
		while (i < shell->data->vec_env.len)
		{
			env = get_env(shell->data, i);
			printf("%s=%s\n", env->key, env->value);
			i++;
		}
		shell->exitcode = 0;
	}
}

static int	is_valid_unset_identifier(const char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_var(t_shell *shell, const char *key)
{
	t_env	*env_var;
	size_t	i;
	t_env	*last;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var && ft_strcmp(env_var->key, key) == 0)
		{
			// free(env_var->key);
			// free(env_var->value);
			if (i < shell->data->vec_env.len - 1)
			{
				last = (t_env *)vec_get(&shell->data->vec_env,
						shell->data->vec_env.len - 1);
				env_var->key = last->key;
				env_var->value = last->value;
				last->key = NULL;
				last->value = NULL;
			}
			else
			{
				env_var->key = NULL;
				env_var->value = NULL;
			}
			shell->data->vec_env.len--;
			return ;
		}
		i++;
	}
}

void	builtin_unset(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	exit_code;

	if (!cmd->argv[1])
	{
		shell->exitcode = 0;
		return ;
	}
	exit_code = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_unset_identifier(cmd->argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_code = 1;
		}
		else
		{
			remove_env_var(shell, cmd->argv[i]);
		}
		i++;
	}
	update_shell_envp(shell);
	update_exitcode(exit_code, shell);
	//shell->exitcode = exit_code;
}
