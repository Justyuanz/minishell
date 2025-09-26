#include "minishell.h"


//' ' = super safe → everything is literal.
//" " = semi safe → things like $ and \" still work.
//You can’t mix ' inside '...'.
//You can put " " inside '...' and vice versa.

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
		printf("unclosed single quote\n"); // unclosed quote error handle
	return (i);
}

size_t handle_double_quote(char *buf, char *line, size_t i, size_t *off)
{
	i++;
	while (line[i] && line[i] != '"')
	{
		buf[*off] = line[i];
		(*off)++;
		i++;
		//if (line[i] == '$')
			//work on expansion...
	}
	if (line[i] == '"')
		i++;
	else
		printf("unclosed single quote\n"); // unclosed quote error handle
	return (i);
}

size_t read_word(t_data *d, char *line, size_t i)
{
	char	buf[1024];
	size_t  off = 0; 

	while (line[i] && line[i] != '>' && line[i] != '<' && line[i] != '|' && !ft_isspace(line[i]))
	{
		if (line[i] == '\'')
		{
			i = handle_single_quote(buf, line, i, &off);
		}
		else if (line[i] == '"')
		{
			i = handle_double_quote(buf, line, i, &off);			
		}
		else
			buf[off++] = line[i++];
	}
		push_tok(d, buf, off, WORD);
	return (i);
}
