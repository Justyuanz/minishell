/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 18:52:45 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/28 19:04:51 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_cmd(t_data *d, t_cmd **cmd, t_vec *argv)
{
	if (vec_new(argv, 1, sizeof(char *)) == -1)
		destroy_and_exit(d, "Vec init cmd fail", 1);
	*cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
	if (!cmd)
		destroy_and_exit(d, "Arena alloc cmd fail", 1);
	if (vec_new(&(*cmd)->redirs, 1, sizeof(t_redir *)) == -1)
		destroy_and_exit(d, "Vec init cmd fail", 1);
	if (vec_new(&(*cmd)->quotes, 1, sizeof(t_quote *)) == -1)
		destroy_and_exit(d, "Vec init cmd fail", 1);
}

static void	init_new_cmd(t_data *d, t_vec *argv, t_cmd **cmd)
{
	if (vec_new(argv, 1, sizeof(char *)) == -1)
		destroy_and_exit(d, "Vec init cmd fail", 1);
	*cmd = (t_cmd *)arena_alloc(&d->arena_tok, sizeof(t_cmd));
	if (!cmd)
		destroy_and_exit(d, "Arena alloc cmd fail", 1);
	if (vec_new(&(*cmd)->redirs, 1, sizeof(t_redir *)) == -1)
		destroy_and_exit(d, "Vec init cmd fail", 1);
	if (vec_new(&(*cmd)->quotes, 1, sizeof(t_quote *)) == -1)
		destroy_and_exit(d, "Vec init cmd fail", 1);
}

static void	handle_last_cmd(t_data *d, t_vec *argv, t_cmd **cmd)
{
	if (vec_push(argv, NULL) == -1)
		destroy_and_exit(d, "Vec push cmd fail", 1);
	(*cmd)->argv = (char **)(*argv).memory;
	if (vec_push(&d->vec_cmds, *cmd) == -1)
		destroy_and_exit(d, "Vec push cmd fail", 1);
}

void	build_vec_cmds(t_data *d)
{
	size_t	i;
	t_token	*tok;
	t_cmd	*cmd;
	t_vec	argv;

	i = ~0;
	init_cmd(d, &cmd, &argv);
	while (++i < d->vec_tok.len)
	{
		tok = get_tok(d, i);
		if (tok->type == WORD)
		{
			vec_push(&argv, tok->str);
			track_quotes(d, tok, cmd);
		}
		else if (tok->type == REDIR_IN || tok->type == REDIR_OUT
			|| tok->type == APPEND || tok->type == HEREDOC)
			handle_redir(d, cmd, tok, &i);
		else if (tok->type == PIPE)
		{
			handle_pipe(d, &argv, cmd);
			init_new_cmd(d, &argv, &cmd);
		}
	}
	handle_last_cmd(d, &argv, &cmd);
}
