#include "minishell.h"

size_t read_pipe(t_data *d, char *line, size_t i, t_quote quote)
{
    if (line[i] == '|')
    {
        push_tok(d, &line[i], 1, PIPE, quote);
        i++;
    }
    return (i);
}

size_t read_redir_operator2(t_data *d, char *line, size_t i, t_quote quote)
{
    if (line[i + 1] == '<')
    {
		if (line[i + 1] && line[i + 2] == '<')
		{
			fprintf(stderr,"syntax error near unexpected token `<'\n");
			exit (EXIT_FAILURE); //cleanup
		}
        push_tok(d, &line[i], 2, HEREDOC, quote);
        i += 2;
    }
    else
    {
        push_tok(d, &line[i], 1, REDIR_IN, quote);
        i++;
    }
    return (i);
}


size_t read_redir_operator(t_data *d, char *line, size_t i, t_quote quote)
{
    if (line[i + 1] == '>' )
    {
		if (line[i + 1] && line[i + 2] == '>')
		{
			fprintf(stderr,"syntax error near unexpected token `>' \n");
			exit (EXIT_FAILURE); //cleanup
		}
        push_tok(d, &line[i], 2, APPEND, quote);
        i += 2;
    }
    else
    {
        push_tok(d, &line[i], 1, REDIR_OUT, quote);
        i++;
    }
    return (i);
}
