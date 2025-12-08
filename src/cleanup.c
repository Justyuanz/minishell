/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:06:26 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/08 14:56:45 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_line(t_data *d)
{
	t_cmd	*cmd;
	size_t	i;

	i = 0;
	if (!d)
		return ;
	while (i < d->vec_cmds.len)
	{
		cmd = get_cmd(d, i);
		if (cmd->argv)
		{
			free(cmd->argv);
			cmd->argv = NULL;
		}
		vec_free(&cmd->redirs);
		i++;
	}
	vec_free(&d->vec_cmds);
	vec_free(&d->vec_tok);
}

void	cleanup_shell(t_data *d)
{
	if (!d)
		return ;
	cleanup_line(d);
	vec_free(&d->vec_env);
	arena_free(&d->arena_env);
	arena_free(&d->arena_tok);
	rl_clear_history();
}

void	cleanup_dupped_files(t_cmd *cmd)
{
	size_t	j;
	t_redir	*redir;

	j = 0;
	while (j < cmd->redirs.len)
	{
		redir = get_redir(cmd, j);
		if (redir->strdupped == 88)
		{
			free(redir->file);
			redir->file = NULL;
		}
		j++;
	}
}

void	cleanup_line_runtime(t_data *d)
{
	size_t	i;
	t_cmd	*cmd;

	if (!d)
		return ;
	i = 0;
	while (i < d->vec_cmds.len)
	{
		cmd = get_cmd(d, i);
		cleanup_dupped_files(cmd);
		if (cmd->argv)
		{
			free(cmd->argv);
			cmd->argv = NULL;
		}
		vec_free(&cmd->redirs);
		vec_free(&cmd->quotes);
		i++;
	}
	vec_reset(&d->vec_cmds);
	vec_reset(&d->vec_tok);
	arena_reset(&d->arena_tok);
}

void	eof_cleanup(t_data *d, t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		free(shell->envp[i]);
		i++;
	}
	free(shell->envp);
	destroy_and_exit(d, NULL, 0);
}
