#include "minishell.h"

void cleanup_line(t_data *d)
{
	t_cmd	*cmd;
	size_t	i;

	i = 0;
	if (!d)
		return;
	while( i < d->vec_cmds.len)
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

void cleanup_shell(t_data *d)
{
	size_t	i;

	i = -1;
	if (!d)
		return;
	cleanup_line(d);
	vec_free(&d->vec_env);
	arena_free(&d->arena_env);
	arena_free(&d->arena_tok);
	rl_clear_history();
}


void cleanup_line_runtime(t_data *d)
{
	size_t	i;
	t_cmd *cmd;

    if (!d)
		return;
	i = 0;
    while (i < d->vec_cmds.len)
	{
       cmd = get_cmd(d, i);
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
