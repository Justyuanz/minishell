#include "minishell.h"

// $$
//$?
//$variable

bool expand_in_heredoc(t_redir *redir)
{
    return(redir && redir->type == HEREDOC && redir->quote.single_ON == false && redir->quote.double_ON == false);
}

bool exit_status(char *line, size_t *i, char *buf, size_t *off)
{
    if (line[*i] == '?')
    {
        int code;
		char *num;
		size_t k;

		k= 0;
		code = ft_shell()->exitcode;
		num = ft_itoa(code);

        while (num[k])
            buf[(*off)++] = num[k++];

        free(num);
        (*i)++; // skip '?'
        return(true);
    }
	return (false);
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
	while (line[*i] && ((line[*i] >= 'a' && line[*i] <= 'z') || (line[*i] >= 'A' && line[*i] <= 'Z')
		|| (line[*i] >= '0' && line[*i] <= '9') || line[*i] == '_' ))
	{
		if((*j) >= 1028)
		{
			tmp[*j] = '\0';
			return;
		}
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
			return ;
		}
		j++;
	}
}

void handle_expansion(t_data *d, char *buf, char *line, size_t *i, size_t *off)
{
	(*i)++;

	if (exit_status(line, i, buf, off))
		return;
	if (bare_dollar(line, buf, off, i))
		return;
	handle_variable(d, buf,line, i ,off);
}

