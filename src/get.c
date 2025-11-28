/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:08:49 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/28 20:18:41 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_cmd_count(t_data *d)
{
	return (d->vec_cmds.len);
}
t_redir	*get_redir(t_cmd *cmd, size_t index)
{
	return ((t_redir *)vec_get(&cmd->redirs, index));
}
t_cmd	*get_cmd(t_data *d, size_t index)
{
	return ((t_cmd *)vec_get(&d->vec_cmds, index));
}

t_token	*get_tok(t_data *d, size_t index)
{
	return ((t_token *)vec_get(&d->vec_tok, index));
}

t_env	*get_env(t_data *d, size_t index)
{
	return ((t_env *)vec_get(&d->vec_env, index));
}

t_quote	*get_quote(t_cmd *cmd, size_t index)
{
	return ((t_quote *)vec_get(&cmd->quotes, index));
}
