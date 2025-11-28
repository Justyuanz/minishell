/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:19:34 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/28 20:57:16 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	read_word(t_data *d, t_buffer buffer, size_t i)
{
	size_t	off;

	if (!d->line || !d->line[i])
		return (i);
	off = 0;
	while (d->line[i] && d->line[i] != '>' && d->line[i] != '<'
		&& d->line[i] != '|' && !ft_isspace(d->line[i]))
	{
		if (d->heredoc_skip == 1)
			read_heredoc_mode(d, &i, &off, &buffer);
		else if (d->line[i] == '\'')
			i = handle_single_quote(d, i, &off, &buffer);
		else if (d->line[i] == '"')
			i = handle_double_quote(d, i, &off, &buffer);
		else if (d->line[i] == '$')
			i = handle_no_quote(d, i, &off, &buffer);
		else
			buffer.buf[off++] = d->line[i++];
	}
	if (off > 0 || buffer.tokquote.single_ON == true || buffer.tokquote.double_ON == true)
		push_tok(d, buf, off, WORD, quote);
	d->heredoc_skip = 0;
	return (i);
}
