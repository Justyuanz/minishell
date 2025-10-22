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

void push_tok(t_data *d, char *line, size_t len, int type)
{
    t_token *tok;

    tok = (t_token *)arena_alloc(&d->arena_tok, sizeof(t_token));
    tok->str = arena_push(&d->arena_tok, line, len + 1);
    tok->str[len] = '\0';
    tok->type = type;
    if (vec_push(&d->vec_tok, tok) == -1)//only push the address of the pointer
		exit (EXIT_FAILURE); //ERROR HANDLING
}

void tokenizer(t_data *d, char *line)
{
    size_t i;
    i = 0;

    while (line[i])
    {
        while(ft_isspace(line[i]))
            i++;
        if(line[i] == '>')
            i = read_redir_operator(d, line, i);
        else if (line[i] == '<')
            i = read_redir_operator2(d, line, i);
        else if (line[i] == '|')
            i = read_pipe(d, line, i);
        else
            i = read_word(d, line, i);
    }
}