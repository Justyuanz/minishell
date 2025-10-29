#include "minishell.h"

void    builtin_pwd(void)
{
    //not sure if it's perfect but seems to be working
    // error handling?
    char cwd[1000000];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("error, cwd");
        return ;
    }
    printf("%s\n", cwd);
}

void    builtin_env(t_shell *shell)
{
	t_env *env;
	size_t i;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env = get_env(shell->data, i);
		printf("%s=%s\n",env->key, env->value);
		i++;
	}
}
/*
void    builtin_export(t_cmd *command, t_shell *shell)
{

       has a lot of edge cases. 
       Will do last

}

void    builtin_unset(t_cmd *command, t_shell *shell)
{

        will get a "key" what to unset from command array.
        need envp for this?
  
}

*/



