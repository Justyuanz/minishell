/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:19:34 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/08 11:35:10 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_redir(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == APPEND);
}

bool	expand_in_heredoc(t_redir *redir)
{
	return (redir && redir->type == HEREDOC && redir->quote.single_on == false
		&& redir->quote.double_on == false);
}

static void	push_word_or_empty(t_data *d, t_buffer *buffer, size_t off)
{
	t_token	*last_tok;

	last_tok = NULL;
	if (d->vec_tok.len > 0)
		last_tok = get_tok(d, (d->vec_tok.len - 1));
	if (off > 0 || buffer->quotes.single_on == true
		|| buffer->quotes.double_on == true)
		push_word_tok(d, off, WORD, buffer);
	else if (d->expanded_empty == 1 && last_tok && (is_redir(last_tok->type)
			|| last_tok->type == PIPE))
	{
		buffer->buf[0] = '\0';
		push_word_tok(d, 0, EMPTY_WORD, buffer);
	}
	d->heredoc_skip = 0;
	d->expanded_empty = 0;
}

size_t	read_word(t_data *d, t_buffer *buffer, size_t i)
{
	size_t	off;

	if (!d->line || !d->line[i])
		return (i);
	off = 0;
	while (d->line[i] && d->line[i] != '>' && d->line[i] != '<'
		&& d->line[i] != '|' && !ft_isspace(d->line[i]))
	{
		if (d->heredoc_skip == 1)
			read_heredoc_mode(d, &i, &off, buffer);
		else if (d->line[i] == '\'')
			i = handle_single_quote(d, i, &off, buffer);
		else if (d->line[i] == '"')
			i = handle_double_quote(d, i, &off, buffer);
		else if (d->line[i] == '$')
			i = handle_no_quote(d, buffer->buf, i, &off);
		else
			buffer->buf[off++] = d->line[i++];
	}
	push_word_or_empty(d, buffer, off);
	return (i);
}
