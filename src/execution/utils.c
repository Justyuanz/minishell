#include "minishell.h"

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0'))
        i++;
    return (s1[i] - s2[i]);
}

void    free_string(char *str)
{
    if (!str)
        return ;
    free(str);
    str = NULL;
}

void    update_exitcode(int error_code, t_shell *shell)
{
    /*
        That will be final exit code after all execution happened.
    */
    if (shell->command_index < 2 || shell->index == shell->command_index)
        shell->exitcode = error_code;
}