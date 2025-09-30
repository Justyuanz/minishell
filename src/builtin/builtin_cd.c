#include "minishell.h"

char *get_env_value(t_shell *shell, char *str)
{
    /*
        will get env value based on "key"
        if we look for PATH (PATH=/usr/local/bin:/usr/bin:/bin)
        return will be  "/usr/local/bin:/usr/bin:/bin"

        will work on this
    */
    return ("/home/user");
}

int cd_home(char *old_pwd, t_shell *shell)
{
    char    *home;

    home = get_env_value(shell, "HOME");
    if (!home || home[0] == '\0')
    {
        ft_putstr_fd("cd: HOME not set\n", 2);
        free_string(old_pwd);
        return (1);
    }
    if (chdir(home) == -1)
    {
        free_string(old_pwd);
        ft_putstr_fd("chdir syscall failed\n", 2);
        update_exitcode(1, shell);
        return (1);
    }
    return (0);
}

int update_old_pwd(char *old_pwd)
{
    /*
        pwd will be home after successful "cd"
    */
}

void    error_cd(int flag, t_shell *shell)
{
    if (flag == 1)
        ft_putstr_fd("cd: too many arguments\n", 2);
    if (flag == 2)
        ft_putstr_fd("No such file or directory\n", 2);
    update_exitcode(1, shell);
}

void    builtin_cd(int i, char **command_array, t_shell *shell)
{
    char *old_pwd;

    while (command_array[i])
        i++;
    if (i > 2)
        return (error_cd(1, shell));
    old_pwd = getcwd(NULL, 0);
    if (command_array[1] == NULL || (ft_strcmp(command_array[1], "--") == 0))
        if (cd_home(old_pwd, shell))
        {
            update_exitcode(1, shell);
            return ;
        }
    else
    {
        if (chdir(command_array[1]) == -1)
        {
            free_string(old_pwd);
            return (error_cd(2, shell));
        }
    }
    update_old_pwd(old_pwd);
}

