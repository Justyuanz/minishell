#include "minishell.h"
size_t read_word(t_data *d, char *line, size_t i)
{
	size_t start;
	// Words may not include unquoted metacharacters. but if quoted it is treated as literal
	start = i;
	while (line[i] && ft_isalnum(line[i]))
		i++;
	if (i > start)
		push_tok(d, &line[start], i - start, WORD);
	return (i);
}

size_t read_pipe(t_data *d, char *line, size_t i)
{
    if (line[i] == '|')
    {
        push_tok(d, &line[i], 1, PIPE);
        i++;
    } 
    return (i);
}

size_t read_env_operator(t_data *d, char *line, size_t i)
{
    if (line[i + 1] == '?' )
    {
        push_tok(d, &line[i], 2, EXPAND);
        i += 2;
    }
    else
    {
        push_tok(d,&line[i], 1, ENV);
        i++;
    }
    return (i);
}

size_t read_redir_operator2(t_data *d, char *line, size_t i)
{
    if (line[i + 1] == '<')
    {
		if (line[i + 1] && line[i + 2] == '<')
		{
			fprintf(stderr,"syntax error near unexpected token `<'\n");
			exit (EXIT_FAILURE); //cleanup
		}
        push_tok(d, &line[i], 2, HEREDOC);
        i += 2;
    }
    else
    {
        push_tok(d, &line[i], 1, REDIR_IN);
        i++;
    }
    return (i);
}


size_t read_redir_operator(t_data *d, char *line, size_t i)
{
    if (line[i + 1] == '>' )
    {
		if (line[i + 1] && line[i + 2] == '>')
		{
			fprintf(stderr,"syntax error near unexpected token `>' \n");
			exit (EXIT_FAILURE); //cleanup
		}
        push_tok(d, &line[i], 2, APPEND);
        i += 2;
    }
    else
    {
        push_tok(d, &line[i], 1, REDIR_OUT);
        i++;
    }
    return (i);
}
