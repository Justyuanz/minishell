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
void handle_redir(t_data *d, t_cmd *cmd, t_token *tok, size_t *i)
{
	t_redir *redir;
    // allocate redir in arena

	redir = (t_redir *)arena_alloc(&d->arena_tok, sizeof(t_redir));
    redir->type = tok->type;
    if ((*i) + 1 < d->vec_tok.len)
    	redir->file = get_tok(d, (*i) + 1)->str;
    else
        redir->file = NULL; // syntax error maybe
    vec_push(&cmd->redirs, redir);
    (*i)++; // skip filename token
}

void handle_pipe(t_data *d, t_vec *argv, t_cmd *cmd)
{
    vec_push(argv, NULL);
    cmd->argv = (char **)(*argv).memory;
    vec_push(&d->vec_cmds, cmd);

    // start new command
    vec_new(argv, 1, sizeof(char *));
    cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
    vec_new(cmd->redirs, 1, sizeof(t_redir *));
}

void init_new_cmd(t_data *d, t_vec *argv, t_cmd **cmd)
{
    vec_new(&argv, 1, sizeof(char *));
    cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
    vec_new((*cmd)->redirs, 1, sizeof(t_redir *));
	(*cmd)->is_builtin = false;
}
void build_vec_cmds(t_data *d)
{
    size_t  i = 0;
    t_token *tok;
    t_cmd   *cmd;
    t_vec   argv;

	//where should i free all the tmp vector here
    vec_new(&d->vec_cmds, 1, sizeof(t_cmd *));
    vec_new(&argv, 1, sizeof(char *));
    cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
    vec_new(&cmd->redirs, 1, sizeof(t_redir *));

    while (i < d->vec_tok.len)
    {
        tok = get_tok(d, i);

        if (tok->type == WORD)
            vec_push(&argv, tok->str);
        else if (tok->type == REDIR_IN || tok->type == REDIR_OUT
              || tok->type == APPEND || tok->type == HEREDOC)
			handle_redir(d, cmd, tok, &i);
        else if (tok->type == PIPE)
			handle_pipe(d, &argv, cmd);
        i++;
    }
    // push last command
    vec_push(&argv, NULL);
    cmd->argv = (char **)argv.memory;
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
