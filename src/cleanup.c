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
		if (cmd->argv)
		{
			free(cmd->argv);
			cmd->argv = NULL;
		}
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


void cleanup_line_runtime(t_data *d)
{
    if (!d) return;

    for (size_t i = 0; i < d->vec_cmds.len; i++) {
        t_cmd *cmd = get_cmd(d, i);

        // Free ONLY the argv array (heap). Do NOT free argv[i] strings (arena-owned).
        if (cmd->argv) {
            free(cmd->argv);
            cmd->argv = NULL;
        }

        // Free the vector buffer for redirs (its elements are arena pointers)
        vec_free(&cmd->redirs);
		vec_free(&cmd->quotes);
    }

    // Reset container lengths (forget elements) but keep their buffers for reuse.
    vec_reset(&d->vec_cmds);
    vec_reset(&d->vec_tok);
}
