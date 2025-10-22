#include "minishell.h"

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0'))
        i++;
    return (s1[i] - s2[i]);
}
void    error_smt(void)
{
    printf("\n\nHello world!");
}
void    update_exitcode(int error_code, t_shell *shell)
{
    /*
        That will be final exit code after all execution happened.
    */
    if (shell->command_index < 2 || shell->index == shell->command_index)
        shell->exitcode = error_code;
}

char *get_env_value(t_shell *shell, char *str)
{
    t_env   *env_var;
    size_t  i;

    i = 0;
    if (!shell || !shell->data || !str)
        return (NULL);
    while(i < shell->data->vec_env.len)
    {
        env_var = (t_env *)vec_get(&shell->data->vec_env, i);
        if (env_var && env_var->key && ft_strcmp(env_var->key, str) == 0)
            return (env_var->value);
        i++;
    }
    return (NULL);
}