/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:29:15 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 19:15:46 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	i = -1;
	while (++i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var && ft_strcmp(env_var->key, key) == 0)
		{
			if (i < shell->data->vec_env.len - 1)
			{
				last = (t_env *)vec_get(&shell->data->vec_env,
						shell->data->vec_env.len - 1);
				if (env_var->key_dupped == 1)
					free(env_var->key);
				if (env_var->value_dupped == 1)
				{
					free(env_var->value);
				}
				env_var->key = last->key;
				env_var->value = last->value;
				last->key = NULL;
				last->value = NULL;
			}
			else
				env_to_null(env_var);
			shell->data->vec_env.len--;
			return ;
		}
	}
}

void	builtin_env(t_shell *shell)
{
	int		i;
	t_cmd	*cmd;

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
		i = 0;
		while (shell->envp && shell->envp[i])
		{
			ft_putendl_fd(shell->envp[i], 1);
			i++;
		}
		shell->exitcode = 0;
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
	i = 0;
	while (cmd->argv[++i])
	{
		if (!is_valid_unset_identifier(cmd->argv[i]))
		{
			ft_putstr_fd("minishell: unset:not a valid identifier\n", 2);
			exit_code = 1;
		}
		else
		{
			remove_env_var(shell, cmd->argv[i]);
			update_shell_envp(shell);
		}
	}
	shell->exitcode = exit_code;
}
