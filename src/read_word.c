#include "minishell.h"


//' ' = super safe → everything is literal.
//" " = semi safe → things like $ and \" still work.
//You can’t mix ' inside '...'.
//You can put " " inside '...' and vice versa.

//loop till encountering single or double quote
//enter handle single or double quote
	//create a tmp buf to store all the word
//maybe need to read $ here?
size_t handle_single_quote(t_data *d, char *line, char *strbuf, size_t i)
{
	size_t off;
	size_t start;

	i++;
	start = i;
	off = 0;
	while (line[i] && line[i] != '\'')
	{
		strbuf[off] = line[i];
		off++;
		i++;
	}
	if (line[i] == '\'')
		i++;
	else
		printf("unclosed single quote\n"); // unclosed quote error handle
	push_tok(d, &line[start], off - start, WORD);
	return (i);
}

size_t handle_double_quote(t_data *d, char *line, char *strbuf, size_t i)
{
	size_t off;
	size_t start;

	i++;
	start = i;
	off = 0;
	while (line[i] && line[i] != '"')
	{
		strbuf[off] = line[i];
		off++;
		i++;
		//if (line[i] == '$')
			//work on expansion...
	}
	if (line[i] == '"')
		i++;
	else
		printf("unclosed single quote\n"); // unclosed quote error handle
	push_tok(d, &line[start], off - start, WORD);
	return (i);
}
size_t handle_no_quote(t_data *d, char *line, char *strbuf, size_t i)
{
	size_t off;
	size_t start;

	start = i;
	off = 0;
	while (line[i] && line[i] != '>' && line[i] != '<' &&
			line[i] != '|' && line[i] != '\'' && line[i] == '"')
	{
		strbuf[off] = line[i]; //copy normal word
		off++;
		i++;
	}
	push_tok(d, &line[start], off -start, WORD);
	return (i);
}
size_t read_word(t_data *d, char *line, size_t i)
{
	char   strbuf[1024];

	while (line[i] && line[i] != '>' && line[i] != '<' && line[i] != '|')
	{
		if (line[i] == '\'') //handle single quote
			i = handle_single_quote(d, line, strbuf, i);
		else if (line[i] == '"')
			i = handle_double_quote(d, line, strbuf, i);
		else
			i = handle_no_quote(d, line, strbuf, i);
	}
	return (i);
}
