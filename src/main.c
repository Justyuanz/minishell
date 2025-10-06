#include "minishell.h"

//updated vec_new vec_push vec_get vec_resize to work with array of ptr

//a func to destroy
//a func to exit?
//tokenizerecho


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
// t_vec vec_cmd (need to store all the WORDS and redirs in to **argv)
// one command vector stores one argv contents(one pipe action)
// remember to add the last argv after the last pipe
// vec_cmd[0] = argv = ("echo", "HI")

// all token before pipe goes into argv
//arena_alloc(arena_tok) t_cmd strcut
//while tok->str !| -> arena push tok->str as argv[i], if |
//argv[i] = NULL
//vec_push(vec_cmd[index], argv[i])

void build_vec_cmds(t_data *d)
{
	t_vec	argv;
	t_token	*tok;
	char	*null;
	size_t	i;

	null = NULL;
	i = 0;
	vec_new(&argv, 1, sizeof(char *));
	while (i < d->vec_tok.len)
	{
		tok = get_tok(d, i);
		if (tok->type == WORD)
		{
			vec_push(&argv, tok->str);
		}
		// else if tok->type == REDIR
		else if (tok->type == PIPE)
		{
			vec_push(&argv, null);                // NULL terminate argv
			t_vec *copy = arena_alloc(&d->arena_tok, sizeof(t_vec)); // allocate space in arena
			*copy = argv;                          // copy the struct
			vec_push(&d->vec_cmds, copy);          // push pointer to vec_cmds
			vec_new(&argv, 1, sizeof(char *));     // start a new argv for next command
		}
		i++;
	}
			vec_push(&argv, null);                // NULL terminate argv
			t_vec *copy = arena_alloc(&d->arena_tok, sizeof(t_vec)); // allocate space in arena
			*copy = argv;                          // copy the struct
			vec_push(&d->vec_cmds, copy);          // push pointer to vec_cmds


	size_t k = 0;
	for (size_t i = 0; i < d->vec_cmds.len; i++)
	{
		t_vec *cmd = get_vec_cmd(d, i);
		fprintf(stderr,"vec_cmd[%zu]\n", i);
				k = 0;
		while (cmd->memory[k]!= NULL)
		{
			fprintf(stderr,"argv[%zu], %s\n",k,  (char *)cmd->memory[k]);
			k++;
		}
	}
	//free argv vec here i guess?
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
		build_vec_cmds(d);
        arena_reset(&d->arena_tok);
        vec_reset(&d->vec_tok);
     }
    free(line);
}

int main(int argc, char **argv, char **envp)
{
    t_data *d;

    (void)argc;
    (void)argv;
    d = get_data();
    shell_init(d, envp);
    while (1)
        read_the_line(d);
    arena_free(&d->arena_tok);
    vec_free(&d->vec_tok);
    return 0;
}
