#include "minishell.h"

// $$
//$?
//$variable
bool exit_status(void)
{
	fprintf(stderr,"exit_status");
	return(true);
}

bool bare_dollar(char *line, char *buf, size_t *off, size_t *i)
{
	if (line[*i] == '$') //$$
	{
		buf[*off] = '$';
		(*off)++;
		(*i)++;
		return (true);
	}
	else if (!line[*i] || ft_isspace(line[*i]) ||line[*i] == '|' || line[*i] == '>' || line[*i] == '<' || line[*i] == '"')
	{
		buf[*off] = '$';
		(*off)++;
		return (true);
	}
	return (false);
}

void store_var_name(char *line, char *tmp, size_t *i, size_t *j)
{
	//a-zA-Z0-9_
	while (line[*i] && ((line[*i] >= 'a' && line[*i] <= 'z') || (line[*i] >= 'A' && line[*i] <= 'Z')
		|| (line[*i] >= '0' && line[*i] <= '9') || line[*i] == '_' ))
	{
		tmp[*j] = line[*i]; //writes the var name
		(*j)++;
		(*i)++;
	}
	tmp[*j] = '\0';
}

void handle_variable(t_data *d, char *buf, char *line, size_t *i, size_t *off)
{
	t_env *env;
	char tmp[1024];
	size_t k;
	size_t j;

	j = 0;
	store_var_name(line, tmp, i, &j);
	k = 0;
	//jinzhang@c2r6p8:~/Github/minishell$ echo $> bash: syntax error near unexpected token `newline'
	j = 0;
	while(j < d->vec_env.len)
	{
		env = get_env(d, j);
		if (str_cmp(tmp, env->key) == true)
		{
			while (k < ft_strlen(env->value))
			{
				buf[*off] = env->value[k];
				(*off)++;
				k++;
			}
		}
		j++;
	}
}

void handle_expansion(t_data *d, char *buf, char *line, size_t *i, size_t *off)
{
	(*i)++; //skip $

	//	exit_status()

	if (bare_dollar(line, buf, off, i))
		return;
	//normal var
	handle_variable(d, buf,line, i ,off);
}

