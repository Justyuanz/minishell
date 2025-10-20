#include"minishell.h"

static void handle_redir(t_data *d, t_cmd *cmd, t_token *tok, size_t *i)
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
static void handle_pipe(t_data *d, t_vec *argv, t_cmd *cmd)
{
	//if (argv_is_empty(argv)) syntax error
    vec_push(argv, NULL);
    cmd->argv = (char **)(*argv).memory;
    vec_push(&d->vec_cmds, cmd);
}

static void init_cmd(t_data *d, t_cmd **cmd, t_vec *argv)
{
	vec_new(&d->vec_cmds, 1, sizeof(t_cmd *));
    vec_new(argv, 1, sizeof(char *));
    *cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
    vec_new(&(*cmd)->redirs, 1, sizeof(t_redir *));
}

static void init_new_cmd(t_data *d, t_vec *argv, t_cmd **cmd)
{
	vec_new(argv, 1, sizeof(char *));
    *cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
    vec_new(&(*cmd)->redirs, 1, sizeof(t_redir *));
	(*cmd)->is_builtin = false;
}

static void handle_last_cmd(t_data *d, t_vec *argv, t_cmd **cmd)
{
	vec_push(argv, NULL);
    (*cmd)->argv = (char **)(*argv).memory;
    vec_push(&d->vec_cmds, *cmd);
}

void build_vec_cmds(t_data *d)
{
    size_t  i;
    t_token *tok;
    t_cmd   *cmd;
    t_vec   argv;

	i = 0;
	// still not sure how and where to free argv here
	init_cmd(d, &cmd, &argv);
    while (i < d->vec_tok.len)
    {
        tok = get_tok(d, i);

        if (tok->type == WORD)
            vec_push(&argv, tok->str);
        else if (tok->type == REDIR_IN || tok->type == REDIR_OUT
              || tok->type == APPEND || tok->type == HEREDOC)
			handle_redir(d, cmd, tok, &i);
        else if (tok->type == PIPE)
		{
			handle_pipe(d, &argv, cmd);
			init_new_cmd(d, &argv, &cmd);
		}
        i++;
    }
    handle_last_cmd(d, &argv, &cmd);
	debug_print_cmds(d);
}
