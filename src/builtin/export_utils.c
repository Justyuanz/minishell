#include "minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_exported_vars(t_shell *shell)
{
	t_env	*env_var;
	size_t	i;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var && env_var->key)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(env_var->key, STDOUT_FILENO);
			if (env_var->value)
			{
				ft_putstr_fd("=\"", STDOUT_FILENO);
				ft_putstr_fd(env_var->value, STDOUT_FILENO);
				ft_putstr_fd("\"", STDOUT_FILENO);
			}
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
}

int	update_env_var(t_shell *shell, char *key, t_env *env_var, char *value)
{
	size_t	i;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var && ft_strcmp(env_var->key, key) == 0)
		{
			env_var->value = value;
			free(key);
			return (0);
		}
		i++;
	}
    return (1);
}

int add_env_var(t_shell *shell, char *key, t_env *env_var, char *value)
{
	env_var = malloc(sizeof(t_env));
	if (!env_var)
	{
		free(key);
		free(value);
		return (1);
	}
	env_var->key = key;
	env_var->value = value;
	if (!vec_push(&shell->data->vec_env, env_var))
	{
		free(key);
		free(value);
		free(env_var);
		return (1);
	}
	return (0);
}
