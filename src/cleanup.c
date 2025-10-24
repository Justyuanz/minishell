#include "minishell.h"
void cleanup_line(t_data *d)
{
	t_cmd	*cmd;
	size_t	i;

	i = -1;
	if (!d)
		return;
	while( ++i < d->vec_cmds.len)
	{
		cmd = get_cmd(d, i);
		vec_free(&cmd->redirs);
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
}
