#include "minishell.h"

bool syntax_error_msg(char *msg, int exitcode)
{
	if(msg)
		ft_putendl_fd(msg, 2);
	ft_shell()->exitcode = exitcode;
	return (false);
}