#include "minishell.h"

t_data *get_data(void)
{
	static t_data d;

	return &d;
}
size_t get_cmd_count(t_data *d)
{
	return(d->vec_cmds.len);
}
t_cmd *get_cmd(t_data *d, size_t index)
{
    return ((t_cmd *)vec_get(&d->vec_cmds, index));
}


t_token *get_tok(t_data *d, size_t index)
{
    return ((t_token *)vec_get(&d->vec_tok, index));
}

t_env *get_env(t_data *d, size_t index)
{
    return ((t_env *)vec_get(&d->vec_env, index));
}
