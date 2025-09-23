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

void push_tok(t_data *d, char *ptr, size_t len, int type)
{
    t_token *tok;

    tok = (t_token *)arena_alloc(&d->arena, sizeof(t_token));
    printf("arena alloc done\n");
    tok->str = arena_push(&d->arena, ptr, len + 1);
    printf("arena push done\n");
    tok->str[len] = '\0';
    tok->type = type;
    printf("\\0 done\n");
    if (vec_push(&d->vec_tok, &tok) == -1)//only push the address of the pointer
        exit (EXIT_FAILURE); //ERROR HANDLING
    printf("vec push done\n");
}

size_t read_operator(char *line, size_t i)
{
    t_data  *d;

    d = get_data();
    if (line[i] == '>')
    {
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
    }
    return (i);
}

// the vector is storing pointers to tokens.
// vec_get returns the ADDRESS of the element slot inside the vector.
// each element is a t_token *, that means the return type here is t_token **.
// casting(t_token **) interpret the returned void * as a t_token **
// * (...) → read the value stored in the slot = the actual t_token *, not the address of the slot 
t_token *get_tok(t_data *d, size_t index)
{
    return (*(t_token **)vec_get(&d->vec_tok, index));
}
void tokenizer(t_data *d, char *line)
{
    size_t i;
    size_t start;
    (void)d;
    i = 0;

    while (line[i])
    {
        while(ft_isspace(line[i]))
            i++;
        start = i;
        if(line[i] == '>' || line[i] == '<' || line[i] == '$'
            || line[i] == '|')
        {
            i = read_operator(line, i);    
        } 
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
        tokenizer(d, line);
    }
    free(line);
     for (size_t j = 0; j < d->vec_tok.len; j++)
     {
        t_token *tok_back = get_tok(d, j);
        printf("tok->str[%zu]:%s tok->type:%d\n", j, tok_back->str, tok_back->type);
     }
    
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