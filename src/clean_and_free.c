#include "minishell.h"

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
