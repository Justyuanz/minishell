#include "minishell.h"

size_t handle_single_quote(char *buf, char *line, size_t i, size_t *off)
{
	i++;
	while (line[i] && line[i] != '\'')
	{
		buf[*off] = line[i];
		(*off)++;
		i++;
	}
	if (line[i] == '\'')
		i++;
	else
		fprintf(stderr, "unclosed single quote\n"); // unclosed quote error handle
	return (i);
}

size_t handle_double_quote(t_data *d, char *buf, char *line, size_t i, size_t *off)
{
	i++; //skip first ""
	while (line[i] && line[i] != '"')
	{
		if (line[i] == '$')
		{
			handle_expansion(d, buf, line, &i, off);
			continue;
		}
		buf[*off] = line[i];
		(*off)++;
		i++;
	}
	if(line[i] == '"')
		i++;
	return (i);
}

size_t handle_no_quote(t_data *d, char *buf, char *line, size_t i, size_t *off)
{
	while (line[i] && !ft_isspace(line[i]))
	{
		if (line[i] == '$')
		{
			handle_expansion(d, buf, line, &i, off);
			continue;;
		}
		buf[*off] = line[i];
		(*off)++;
		i++;
	}
	return (i);
}
size_t read_word(t_data *d, char *line, size_t i)
{
	char	buf[1024]; //guard here
	size_t  off;
	size_t quote_flag;

	off = 0;
	quote_flag = 0;
	while (line[i] && line[i] != '>' && line[i] != '<' && line[i] != '|' && !ft_isspace(line[i]))
	{
		if (line[i] == '\'')
		{
			quote_flag = 1;
			i = handle_single_quote(buf, line, i, &off);
		}
		else if (line[i] == '"')
		{
			quote_flag = 1;
			i = handle_double_quote(d, buf, line, i, &off);
		}
		else if (line[i] == '$')
			i = handle_no_quote(d, buf, line, i, &off);
		else
			buf[off++] = line[i++];
	}
	if (off > 0 || quote_flag == 1)
		push_tok(d, buf, off, WORD);
	return (i);
}

