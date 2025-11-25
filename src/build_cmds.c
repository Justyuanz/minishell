#include"minishell.h"

static size_t	count_words(char const *s)
{
	size_t	len;
	size_t	i;
	size_t	word;

	i = 0;
	word = 0;
	len = ft_strlen(s);
	while (i < len)
	{
		if ((ft_isspace(s[i]) && !ft_isspace(s[i + 1])) || s[i + 1] == '\0')
			word++;
		i++;
	}
	return (word);
}
static void is_ambigurous_redir(t_redir *redir)
{
	if (!redir->quote.single_ON && !redir->quote.double_ON)
    {
        if (count_words(redir->file) > 1)
		{
            redir->is_ambiguous = true;
			ft_putendl_fd("mini: ambiguous redirection", 2);
			ft_shell()->exitcode = 1;
		}
    }
        if (!redir->file || redir->file[0] == '\0')
		{
            redir->is_ambiguous = true;
			ft_putendl_fd("mini: ambiguous redirection", 2);
			ft_shell()->exitcode = 1;
		}
	}

static void handle_redir(t_data *d, t_cmd *cmd, t_token *tok, size_t *i)
{
	t_token *next_tok;
	t_redir *redir;

	next_tok = get_tok(d, (*i) + 1);
	redir = (t_redir *)arena_alloc(&d->arena_tok, sizeof(t_redir));
    redir->type = tok->type;
	redir->is_ambiguous = false;
    if ((*i) + 1 < d->vec_tok.len)
	{
    	redir->file = next_tok->str;
		redir->quote = next_tok->quote;
		is_ambigurous_redir(redir);
	}
	else
		redir->file = NULL; // syntax error maybe
    vec_push(&cmd->redirs, redir);
    (*i)++;
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
    vec_new(argv, 1, sizeof(char *));
    *cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
    vec_new(&(*cmd)->redirs, 1, sizeof(t_redir *));
	vec_new(&(*cmd)->quotes, 1, sizeof(t_quote *));
}

static void init_new_cmd(t_data *d, t_vec *argv, t_cmd **cmd)
{
	vec_new(argv, 1, sizeof(char *));
    *cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
    vec_new(&(*cmd)->redirs, 1, sizeof(t_redir *));
	vec_new(&(*cmd)->quotes, 1, sizeof(t_quote *));
}

static void handle_last_cmd(t_data *d, t_vec *argv, t_cmd **cmd)
{
	vec_push(argv, NULL);
    (*cmd)->argv = (char **)(*argv).memory; //leak from here
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
		{
            vec_push(&argv, tok->str);
			t_quote *q = arena_alloc(&d->arena_tok, sizeof(t_quote));
			*q = tok->quote;
			vec_push(&cmd->quotes, q);

		}
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
}
