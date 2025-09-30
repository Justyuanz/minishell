#include "minishell.h"
//loop through envp, try to print them out,
//push env struct to arena, the the strings

// $$
//$?
//$variable
bool bare_dollar(char *line, char *buf, size_t *off, size_t *i)
{
	if (!line[*i] || ft_isspace(line[*i]) ||  line[*i] == '|' || line[*i] == '>' || line[*i] == '<')
	{
		buf[*off] = '$';
		(*off)++;
		return (true);
	}
	return (false);
}
void handle_expansion(t_data *d, char *buf, char *line, size_t *i, size_t *off)
{
	t_env *env;
	char tmp[1024];
	size_t j = 0;

	(*i)++;
	//if line[*i] == ?
	//if line[*i] == $
	if (double_dollor(line, buf, off, i))
	//if only bare $
	if (bare_dollar(line, buf, off, i))
		return;
	//normal var
	while (line[*i] && line[*i] != '>' && line[*i] != '<' && line[*i] != '|' && !ft_isspace(line[*i]) && line[*i] != '"')
	{
		tmp[j] = line[*i]; //writes the var name
		j++;
		(*i)++;
	}
	//jinzhang@c2r6p8:~/Github/minishell$ echo $> bash: syntax error near unexpected token `newline'
	tmp[j] = '\0';
	j = 0;
	for (size_t k = 0; k < d->vec_env.len; k++)
	{
		env = get_env(d, k);
		if (ft_strncmp(tmp, env->key, ft_strlen(env->key)) == 0)
		{
			for (size_t h = 0; h < ft_strlen(env->value); h++)
			{
				buf[*off] = env->value[h];
				(*off)++;
			}
		}
	}
}
