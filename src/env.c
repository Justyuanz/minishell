#include "minishell.h"

//loop through envp, try to print them out,
//push env struct to arena, the the strings

// $$
//$?
//$variable

void handle_expansion(t_data *d, char *buf, char *line, size_t *i, size_t *off)
{
	t_env *env;
	char tmp[1024];
	size_t j = 0;

	(*i)++;
	while (line[*i] && line[*i] != '>' && line[*i] != '<' && line[*i] != '|' && !ft_isspace(line[*i]) && line[*i] != '"')
	{
		tmp[j] = line[*i];
		j++;
		(*i)++;
	}
	//jinzhang@c2r6p8:~/Github/minishell$ echo $> bash: syntax error near unexpected token `newline'
	tmp[j] = '\0';
	j = 0;
	for (size_t k = 0; k < d->vec_env.len; k++)
	{
		env = get_env(d, k);
		if (ft_strncmp(tmp, env->key, ft_strlen(tmp)) == 0)
		{
			for (size_t h = 0; h < ft_strlen(env->value); h++)
			{
				buf[*off] = env->value[h];
				(*off)++;
			}
		}
		else
		{
			buf[*off] = tmp[j];
			(*off)++;
			j++;
		}
	}

}

void push_env(t_data *d, char **envp, size_t i)
{
	char	*str;
	char	*equal;
    t_env	*env;

	str = arena_push(&d->arena_env, envp[i], ft_strlen(envp[i]) + 1);
	equal = ft_strchr(str, '='); //returns the address of str starting from equal sign
	if (equal)
	{
		*equal = '\0';
    	env = (t_env *)arena_alloc(&d->arena_env, sizeof(t_env));
    	env->key = str;
		env->value = equal + 1;
    	if (vec_push(&d->vec_env, env) == -1)//only push the address of the struct
			exit (EXIT_FAILURE); //ERROR HANDLING
	}
}

void envp_init(t_data *d, char **envp)
{
	size_t i;

	i = 0;
	if (!envp)
		return; //error & free
	while (envp[i])
	{
		push_env(d, envp, i);
		i++;
	}
// 	for (size_t j = 0; j < d->vec_env.len; j++)
// 	{
// 		t_env *env_back = get_env(d, j);
// 		fprintf(stderr,"env->key:%s \nenv->value:%s \n", env_back->key, env_back->value);
// 		fprintf(stderr,"--------------------------------------\n");
// 	}
 }