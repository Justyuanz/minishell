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
void debug_print_cmds(t_data *d)
{
    for (size_t i = 0; i < d->vec_cmds.len; i++)
    {
        t_cmd *cmd = (t_cmd *)vec_get(&d->vec_cmds, i);
        fprintf(stderr, "cmd[%zu]:\n", i);
        for (size_t j = 0; cmd->argv[j]; j++)
		{
            fprintf(stderr, "  argv[%zu]: %s\n", j, cmd->argv[j]);
		}
    }
}
void build_vec_cmds(t_data *d)
{

    size_t  i = 0;
    t_token *tok;
    t_cmd   *cmd;
    t_vec   argv;
    char    *null = NULL;

    vec_new(&d->vec_cmds, 1, sizeof(t_cmd *)); // vector of cmd pointers
    vec_new(&argv, 1, sizeof(char *));          // temp argv holder
    vec_new(&cmd->redirs, 1, sizeof(void *)); // empty for now
    // iterate tokens
    while (i < d->vec_tok.len)
    {
        tok = get_tok(d, i);

        if (tok->type == WORD)
            vec_push(&argv, tok->str);
        //if tok->type == redirs should i store the redirs to a tmp holder here first
		//how can i make sure that it belongs to the same cmd group
		else if (tok->type == REDIR_IN || tok->type == REDIR_OUT || tok->type == APPEND || tok->type == HEREDOC)
		{
			
			//save the type,  check for the next element and push if it is a string
		}
		else if (tok->type == PIPE)
        {
            // terminate argv with NULL
            vec_push(&argv, null);

            // allocate a command
            cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
            cmd->argv = (char **)argv.memory;
            cmd->is_builtin = false; // default

            // push cmd pointer into main vector
            vec_push(&d->vec_cmds, cmd);

            // start a new argv for next command
            vec_new(&argv, 1, sizeof(char *));
        }
        i++;
    }

    // handle last command
    vec_push(&argv, null);
    cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
    cmd->argv = (char **)argv.memory;
    cmd->is_builtin = false;
    vec_new(&cmd->redirs, 1, sizeof(void *));
    vec_push(&d->vec_cmds, cmd);
    debug_print_cmds(d);
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
