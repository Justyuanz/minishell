/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmds_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:05:27 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 15:32:37 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_words(char const *s)
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

void	track_quotes(t_data *d, t_token *tok, t_cmd *cmd)
{
	t_quote	*q;

	q = arena_alloc(d, &d->arena_tok, sizeof(t_quote));
	if (!q)
		destroy_and_exit(d, "Arena alloc tok fail", 1);
	*q = tok->quote;
	if (vec_push(&cmd->quotes, q) == -1)
		destroy_and_exit(d, "Vec push  quote fail", 1);
}

void	is_ambigurous_redir(t_redir *redir)
{
	if (!redir->quote.single_on && !redir->quote.double_on)
	{
		if (count_words(redir->file) > 1)
		{
			redir->is_ambiguous = true;
			parse_error_msg(ERROR_MSG_AMBIGUOUS, redir->file, 1);
		}
	}
	if (!redir->file || redir->file[0] == '\0')
	{
		redir->is_ambiguous = true;
		parse_error_msg(ERROR_MSG_AMBIGUOUS, redir->file, 1);
	}
}

void	handle_redir(t_data *d, t_cmd *cmd, t_token *tok, size_t *i)
{
	t_token	*next_tok;
	t_redir	*redir;

	next_tok = get_tok(d, (*i) + 1);
	redir = (t_redir *)arena_alloc(d, &d->arena_tok, sizeof(t_redir));
	if (!redir)
		destroy_and_exit(d, "Arena alloc redir fail", 1);
	redir->type = tok->type;
	redir->is_ambiguous = false;
	if ((*i) + 1 < d->vec_tok.len)
	{
		redir->file = next_tok->str;
		redir->quote = next_tok->quote;
		is_ambigurous_redir(redir);
	}
	else
		redir->file = NULL;
	if (vec_push(&cmd->redirs, redir) == -1)
		destroy_and_exit(d, "Vec push redir fail", 1);
	(*i)++;
}

void	handle_pipe(t_data *d, t_vec *argv, t_cmd *cmd)
{
	if (vec_push(argv, NULL) == -1)
		destroy_and_exit(d, "Vec push argv fail", 1);
	cmd->argv = (char **)(*argv).memory;
	if (vec_push(&d->vec_cmds, cmd) == -1)
		destroy_and_exit(d, "Vec push cmd fail", 1);
}
