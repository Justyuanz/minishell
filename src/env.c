#include "minishell.h"

void push_env(t_data *d, char **envp, size_t i)
{
	char	*str;
	char	*equal;
    t_env	*env;

	str = arena_push(&d->arena_env, envp[i], ft_strlen(envp[i]) + 1);
	if(!str)
		destroy_and_exit(d, "Arena push env fail", 1);
	equal = ft_strchr(str, '='); //returns the address of str starting from equal sign
	if (equal)
	{
		*equal = '\0';
    	env = (t_env *)arena_alloc(&d->arena_env, sizeof(t_env));
		if(!env)
			destroy_and_exit(d, "Arena alloc env fail", 1);
    	env->key = str;
		env->value = equal + 1;
    	if (vec_push(&d->vec_env, env) == -1)//only push the address of the struct
			destroy_and_exit(d, "Vec push env fail", 1);
	}
}

void envp_init(t_data *d, char **envp)
{
	size_t i;

	i = 0;
	if (!envp)
		return;
	while (envp[i])
	{
		push_env(d, envp, i);
		i++;
	}
 }