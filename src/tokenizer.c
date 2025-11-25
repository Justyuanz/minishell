#include "minishell.h"



char *tok_type(t_token_type tok_type)
{
    if (tok_type == WORD)
        return ("WORD");
    else if (tok_type == PIPE)
        return ("PIPE");
    else if (tok_type == REDIR_IN)
        return ("REDIR_IN");
    else if (tok_type == APPEND)
        return ("APPEND");
    else if (tok_type == REDIR_OUT)
        return ("REDIR_OUT");
    else if (tok_type == HEREDOC)
        return ("HEREDOC");
    else if (tok_type == ENV)
        return ("ENV");
    else if (tok_type == EXPAND)
        return ("EXPAND");
    return ("no type");
}

void push_tok(t_data *d, char *line, size_t len, int type, t_quote quote)
{
    t_token *tok;

    tok = (t_token *)arena_alloc(&d->arena_tok, sizeof(t_token));
	if(!tok)
		destroy_and_exit(d, "Arena alloc tok fail", 1);
    tok->str = arena_push(&d->arena_tok, line, len + 1);
	if(!tok->str)
		destroy_and_exit(d, "Arena push tok fail", 1);
    tok->str[len] = '\0';
    tok->type = type;
	tok->quote.single_ON = quote.single_ON;
	tok->quote.double_ON = quote.double_ON;
    if (vec_push(&d->vec_tok, tok) == -1)//only push the address of the pointer
		destroy_and_exit(d, "Vec push tok fail\n", 1);
}

bool tokenizer(t_data *d, char *line)
{
    size_t i;
	t_quote	quote;

	if (!line)
		return(false);
	i = 0;
	quote.single_ON = false;
	quote.double_ON = false;
	d->unclosed_quote = 0;
    while (line[i])
    {
        while(ft_isspace(line[i]))
            i++;
        if(line[i] == '>')
            i = read_redir_operator(d, line, i, quote);
        else if (line[i] == '<')
            i = read_redir_operator2(d, line, i, quote);
        else if (line[i] == '|')
            i = read_pipe(d, line, i, quote);
        else
		{
			if (ft_strlen(line + i) > WORD_BUF_SIZE)
				return(parse_error_msg("mini: line too long", NULL, 1));
            i = read_word(d, line, i, quote);
		}
		if (d->unclosed_quote == 1)
			break;
    }
	return (true);
}
