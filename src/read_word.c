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
	fprintf(stderr,"line[i]:%c handle_double_quote\n", line[i]);
	while (line[i] && line[i] != '"')
	{
		if (line[i] == '$')
		{
			handle_expansion(d, buf, line, &i, off);
		}
		fprintf(stderr,"line[i]:%c handle_double_quote inside loop\n", line[i]);
		buf[*off] = line[i];
		(*off)++;
		i++;
	}

	return (i);
}
size_t handle_no_quote(t_data *d, char *buf, char *line, size_t i, size_t *off)
{
	while (line[i])
	{
		if (line[i + 1])
		{
			handle_expansion(d, buf, line, &i, off);
			break;
		}
		else
		{
			buf[*off] = line[i];
			(*off)++;
			i++;
		}
		buf[*off] = line[i];
		(*off)++;
		i++;
	}
	return (i);
}
size_t read_word(t_data *d, char *line, size_t i)
{
	char	buf[1024];
	size_t  off;
	size_t quote_flag;

	off = 0;
	quote_flag = 0;
	while (line[i] && line[i] != '>' && line[i] != '<' && line[i] != '|' && !ft_isspace(line[i]))
	{
		if (line[i] == '\'')
		{
			quote_flag = 1;
			fprintf(stderr,"single ' quote line[%zu]:%c\n", i, line[i]);
			i = handle_single_quote(buf, line, i, &off);
		}
		else if (line[i] == '"')
		{
			fprintf(stderr,"double quote line[%zu]:%c\n", i, line[i]);
			quote_flag = 1;
			i = handle_double_quote(d, buf, line, i, &off);
		}
		else if (line[i] == '$')
		{
			fprintf(stderr,"$ line[%zu]:%c\n", i, line[i]);
			i = handle_no_quote(d, buf, line, i, &off);
		}
		else
		{
			fprintf(stderr,"others line[%zu]:%c\n", i, line[i]);
			buf[off++] = line[i++];
		}
	}
	if (off > 0 || quote_flag == 1)
	{
		fprintf(stderr,"line[i]:%s push buf\n", buf);
		push_tok(d, buf, off, WORD);
	}
	return (i);
}

