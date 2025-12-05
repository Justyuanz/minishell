/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:19:34 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/06 00:53:44 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_redir(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == APPEND);
}
size_t	read_word(t_data *d, t_buffer *buffer, size_t i)
{
	size_t	off;
	t_token *last_tok;

	if (!d->line || !d->line[i])
		return (i);
	off = 0;
	last_tok = NULL;
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
	if (d->vec_tok.len > 0)
		last_tok = get_tok(d, (d->vec_tok.len - 1));
	if (off > 0 || buffer->quotes.single_on == true
		|| buffer->quotes.double_on == true)
		push_word_tok(d, off, WORD, buffer);
	else if (d->expanded_empty == 1 && last_tok && is_redir(last_tok->type))
    	push_word_tok(d, 0, WORD, buffer);
	d->heredoc_skip = 0;
	d->expanded_empty = 0;
	return (i);
}
