#include "minishell.h"

static bool check_start_or_end_pipe(t_data *d)
{
    if (get_tok(d, 0)->type == PIPE || get_tok(d, (d->vec_tok.len - 1))->type == PIPE)
		return(syntax_error_msg(SYNTAX_ERROR_PIPE, 2));
    return (true);
}

static bool check_end_with_redirs(t_data *d, size_t i)
{
	t_token *tok;

	tok = get_tok(d, i);
	if ((tok->type == REDIR_IN || tok->type == REDIR_OUT || tok->type == APPEND
			|| tok->type == HEREDOC))
		return(syntax_error_msg(SYNTAX_ERROR_NEWLINE, 2));
	return (true);
}

static bool check_redir_sequence(t_data *d)
{
	t_token *tok;
	t_token *next_tok;
	size_t	i;

	i = 0;
	while (i < d->vec_tok.len - 1)
	{
		tok = get_tok(d, i);
		next_tok = get_tok(d, i + 1);
		if (tok->type == REDIR_IN || tok->type == REDIR_OUT || tok->type == APPEND
			|| tok->type == HEREDOC)
			if (next_tok->type != WORD)
				return(syntax_error_msg(SYNTAX_ERROR_REDIR, 2));
		i++;
	}
	if(!check_end_with_redirs(d, i))
		return (false);

	return (true);
}

static bool check_pipe_sequence(t_data *d)
{
	t_token *tok;
	t_token *next_tok;
	size_t	i;

	i = 0;
	while (i < d->vec_tok.len - 1)
	{
		tok = get_tok(d, i);
		next_tok = get_tok(d, i + 1);
		if (tok->type == PIPE)
			if (next_tok->type == PIPE)
				return(syntax_error_msg(SYNTAX_ERROR_PIPE, 2));
		i++;
	}
	return (true);
}
bool syntax_validation(t_data *d)
{
	if (d->vec_tok.len == 0)
		return(true);
	if (d->unclosed_quote == 1)
		return(syntax_error_msg(SYNTAX_ERROR_QUOTE, 2));
	if (!check_start_or_end_pipe(d) || !check_redir_sequence(d) || !check_pipe_sequence(d))
        return (false);

    return (true);
}