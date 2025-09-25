#include "minishell.h"

//updated vec_new vec_push vec_get vec_resize to work with array of ptr

//a func to destroy
//a func to exit?
//tokenizerecho

t_data *get_data(void)
{
	static t_data d;

	return &d;
}
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

t_token *get_tok(t_data *d, size_t index)
{
    return ((t_token *)vec_get(&d->vec_tok, index));
}
void debug_print_tokens(t_data *d)
{
    t_token *tok;
    size_t  j;

    j = 0;
    fprintf(stderr, "\n===============TOKEN INFO==============\n");
    fprintf(stderr,"vec_tok.len = %zu\n\n", d->vec_tok.len);
    while (j < d->vec_tok.len)
    {
        tok = get_tok(d, j);
        fprintf(stderr, "vec_tok.memory[%zu] -> tok =%p\n", j, (void *)tok);
        fprintf(stderr,"            tok->str  :  \"%s\"\n",tok->str);
        fprintf(stderr,"            tok->type :  %s\n", tok_type(tok->type));
        fprintf(stderr,"--------------------------------------\n");
        j++;
    }
        fprintf(stderr, "\n===============END INFO==============\n");
}

void push_tok(t_data *d, char *line, size_t len, int type)
{
    t_token *tok;

    tok = (t_token *)arena_alloc(&d->arena, sizeof(t_token));
    tok->str = arena_push(&d->arena, line, len + 1);
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
        else if (line[i] == '$')
            i = read_env_operator(d, line, i);
        else if (line[i] == '|')
            i = read_pipe(d, line, i);
        else
            i = read_word(d, line, i);
    }
}

void read_the_line(t_data *d)
{
    char  *line;

    line = readline("minishell$");
    if (!line)
        exit(EXIT_FAILURE); //error handling
    if (*line)
    {
        add_history(line);
        tokenizer(d, line);
        debug_print_tokens(d); //for testing
        arena_reset(&d->arena);
        vec_reset(&d->vec_tok);
     }
    free(line);
}

int main(void)
{
    t_data *d;

    d = get_data();
    shell_init(d);
    while (1)
        read_the_line(d);
    arena_free(&d->arena);
    vec_free(&d->vec_tok);
    return 0;
}
