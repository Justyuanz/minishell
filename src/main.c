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

void debug_print_tokens(t_data *d)
{
    fprintf(stderr, "\n=== TOKENS IN VECTOR ===\n");
    fprintf(stderr, "vec_tok.len = %zu\n", d->vec_tok.len);
    for (size_t j = 0; j < d->vec_tok.len; j++)
    {
         // memory[j] is a void*, cast back to t_token*
        t_token *tok = (t_token *)d->vec_tok.memory[j];
        fprintf(stderr, "vec_tok.memory[%zu] -> tok=%p\n", j, (void *)tok);
        fprintf(stderr, "    tok->str  = \"%s\"\n", tok->str);
        fprintf(stderr, "    tok->type = %d\n", tok->type);
    }
    fprintf(stderr, "=========================\n\n");
}

t_token *get_tok(t_data *d, size_t index)
{
    return ((t_token *)vec_get(&d->vec_tok, index));
}

bool ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

void shell_init(t_data *d)
{
	//perhaps more things to init here
	if (arena_init(&d->arena, 10000) == -1)
		exit(EXIT_FAILURE);
	if (vec_new(&d->vec_tok, 1, sizeof(t_token *)) == -1)
	{
		arena_free(&d->arena);
		exit(EXIT_FAILURE);
	}
	d->q.single_ON = false;
	d->q.double_ON = false;

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

size_t read_pipe(char *line, size_t i)
{
    t_data  *d;

    d = get_data();
    if (line[i] == '|' )
    {
        push_tok(d, &line[i], 2, PIPE);
        i++;
    }
    
    return (i);
}
size_t read_env_operator(char *line, size_t i)
{
    t_data  *d;

    d = get_data();
    if (line[i + 1] == '?' )
    {
        push_tok(d, &line[i], 3, EXPAND);
        i += 2;
    }
    else
    {
        push_tok(d,&line[i], 2, ENVIRONMENT);
        i++;
    }
    
    return (i);
}
size_t read_redir_operator2(char *line, size_t i)
{
    t_data  *d;

    d = get_data();
    if (line[i + 1] == '<')
    {
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


size_t read_redir_operator(char *line, size_t i)
{
    t_data  *d;

    d = get_data();
    if (line[i + 1] == '>' )
    {
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

void tokenizer(char *line)
{
    size_t i;
    size_t start;
    i = 0;

    while (line[i])
    {
        while(ft_isspace(line[i]))
            i++;
        start = i;
        if(line[i] == '>')
            i = read_redir_operator(line, i);
        else if (line[i] == '<')
            i = read_redir_operator2(line, i);
        else if (line[i] == '$')
            i = read_env_operator(line, i);
        else if (line[i] == '|')
            i = read_pipe(line, i);
       // else if (line[i] == '\'' || line[i] == '"')
         //   i = read_quote(line, i);
        else
            i ++;
    }
}

void read_the_line(t_data *d)
{
    char  *line;

    line = readline("minishell\n$");
    if (!line)
        exit(EXIT_FAILURE); //error handling
    if (*line)
    {
        add_history(line);
        tokenizer(line);
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

/*
• Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
• Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
• Implement the following redirections:
◦ < should redirect input.
◦ > should redirect output.
◦ << should be given a delimiter, then read the input until a line containing the
delimiter is seen. However, it doesn’t have to update the history!
◦ >> should redirect output in append mode.
• Implement pipes (| character). The output of each command in the pipeline is
connected to the input of the next command via a pipe.
• Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.
• Handle $? which should expand to the exit status of the most recently executed
foreground pipeline.
*/

//' ' = super safe → everything is literal.
//" " = semi safe → things like $ and \" still work.
//You can’t mix ' inside '...'.
//You can put " " inside '...' and vice versa.




/*
• Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
• Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
• Implement the following redirections:
◦ < should redirect input.
◦ > should redirect output.
◦ << should be given a delimiter, then read the input until a line containing the
delimiter is seen. However, it doesn’t have to update the history!
◦ >> should redirect output in append mode.
• Implement pipes (| character). The output of each command in the pipeline is
connected to the input of the next command via a pipe.
• Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.
• Handle $? which should expand to the exit status of the most recently executed
foreground pipeline.
*/