#include "minishell.h"

void    builtin_pwd(void)
{
    //not sure if it's perfect but seems to be working
    // error handling?
    char cwd[1000000];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("error, cwd");
        return ;
    }
    printf("%s\n", cwd);
}

void    builtin_env(t_shell *shell)
{
	t_env *env;
	size_t i;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env = get_env(shell->data, i);
		printf("%s=%s\n",env->key, env->value);
		i++;
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

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var && ft_strcmp(env_var->key, key) == 0)
		{
			//free(env_var->key);
			//free(env_var->value);
			if (i < shell->data->vec_env.len - 1)
			{
				t_env *last = (t_env *)vec_get(&shell->data->vec_env, shell->data->vec_env.len - 1);
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
			return;
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
		return;
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
	shell->exitcode = exit_code;
}
