/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:26:18 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/03 23:26:41 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*tok_type(t_token_type tok_type)
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

void	push_word_tok(t_data *d, size_t len, int type, t_buffer *buffer)
{
	t_token	*tok;

	tok = (t_token *)arena_alloc(d, &d->arena_tok, sizeof(t_token));
	if (!tok)
		destroy_and_exit(d, "Arena alloc tok fail", 1);
	tok->str = arena_push(&d->arena_tok, buffer->buf, len + 1);
	if (!tok->str)
		destroy_and_exit(d, "Arena push tok fail", 1);
	tok->str[len] = '\0';
	tok->type = type;
	tok->quote.single_ON = buffer->quotes.single_ON;
	tok->quote.double_ON = buffer->quotes.double_ON;
	if (vec_push(&d->vec_tok, tok) == -1)
		// only push the address of the pointer
		destroy_and_exit(d, "Vec push tok fail\n", 1);
}

static bool	token_loop(t_data *d, t_buffer *buffer)
{
	size_t	i;

	i = 0;
	buffer->quotes = (t_quote){false, false};
	d->unclosed_quote = 0;
	while (d->line[i])
	{
		while (ft_isspace(d->line[i]))
			i++;
		if (d->line[i] == '>')
			i = read_redir_operator(d, i);
		else if (d->line[i] == '<')
			i = read_redir_operator2(d, i);
		else if (d->line[i] == '|')
			i = read_pipe(d, i);
		else
		{
			if (ft_strlen(d->line + i) > WORD_BUF_SIZE)
				return (parse_error_msg("mini: line too long", NULL, 1));
			i = read_word(d, buffer, i);
		}
		if (d->unclosed_quote == 1)
			break ;
	}
	return (true);
}

bool	tokenizer(t_data *d)
{
	t_buffer	buffer;

	if (!d->line)
		return (false);
	ft_bzero(&buffer, sizeof(t_buffer));
	return (token_loop(d, &buffer));
}
