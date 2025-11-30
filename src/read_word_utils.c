/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_word_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:20:53 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:25:56 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	handle_single_quote(t_data *d, size_t i, size_t *off, t_buffer *buffer)
{
	if (!d->line[i])
		return (i);
	buffer->quotes.single_ON = true;
	i++;
	while (d->line[i] && d->line[i] != '\'')
	{
		buffer->buf[*off] = d->line[i];
		(*off)++;
		i++;
	}
	if (d->line[i] == '\'')
		i++;
	else
		d->unclosed_quote = 1;
	return (i);
}

size_t	handle_double_quote(t_data *d, size_t i, size_t *off, t_buffer *buffer)
{
	if (!d->line[i])
		return (i);
	buffer->quotes.double_ON = true;
	i++;
	while (d->line[i] && d->line[i] != '"')
	{
		if (d->line[i] == '$')
		{
			handle_expansion(d, buffer->buf, &i, off);
			continue ;
		}
		buffer->buf[*off] = d->line[i];
		(*off)++;
		i++;
	}
	if (d->line[i] == '"')
		i++;
	else
		d->unclosed_quote = 1;
	return (i);
}

size_t	handle_no_quote(t_data *d, char *buf, size_t i, size_t *off)
{
	if (!d->line[i])
		return (i);
	while (d->line[i] && !ft_isspace(d->line[i]) && d->line[i] != '<'
		&& d->line[i] != '>' && d->line[i] != '|' && d->line[i] != '\''
		&& d->line[i] != '"')
	{
		if (d->line[i] == '$')
		{
			handle_expansion(d, buf, &i, off);
			continue ;
		}
		buf[*off] = d->line[i];
		(*off)++;
		i++;
	}
	return (i);
}

void	read_heredoc_mode(t_data *d, size_t *i, size_t *off, t_buffer *buffer)
{
	if (d->line[*i] == '"')
	{
		buffer->quotes.double_ON = true;
		(*i)++;
		while (d->line[*i] && d->line[*i] != '"')
		{
			buffer->buf[*off] = d->line[*i];
			(*off)++;
			(*i)++;
		}
		if (d->line[*i] == '"')
			(*i)++;
		else
			d->unclosed_quote = 1;
	}
	else if (d->line[*i] == '\'')
		(*i) = handle_single_quote(d, (*i), off, buffer);
	else
		buffer->buf[(*off)++] = d->line[(*i)++];
}
