/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:06:26 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/10 15:05:49 by jinzhang         ###   ########.fr       */
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

void	cleanup_env_vars(t_shell *shell)
{
	size_t	i;
	t_env	*env_var;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var)
		{
			if (env_var->key_dupped && env_var->key)
				free(env_var->key);
			if (env_var->value_dupped && env_var->value)
				free(env_var->value);
		}
		i++;
	}
	// shell->data->vec_env.len = 0;
	if (shell && shell->data)
		shell->data->vec_env.len = 0;
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
		redir->strdupped = 0;
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
	ft_shell()->single_builtin = 0;
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
	if (shell->pids)
	{
		free(shell->pids);
		shell->pids = NULL;
	}
	close(shell->savestdout);
	close(shell->savestdin);
	cleanup_env_vars(shell);
	destroy_and_exit(d, NULL, shell->exitcode);
}
