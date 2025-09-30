#include "minishell.h"

void    builtin_pwd(t_command *command, t_shell *shell)
{
    /*
        can i somehow combine env and pwd??
        i have no idea how to do this rn.
    */
}
void    builtin_env(t_command *command, t_shell *shell)
{
    /*
        can i just print envp??
    */
}

void    builtin_export(t_command *command, t_shell *shell)
{
    /*
        what is the difference between env and export??
    */
}

void    builtin_unset(t_command *command, t_shell *shell)
{
    /*
        will get a "key" what to unset from command array.
        need envp for this?
    */
}

