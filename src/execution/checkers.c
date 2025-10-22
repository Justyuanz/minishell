#include "minishell.h"

int	check_access(const char *full_path, int *found)
{
	if (access(full_path, X_OK) == 0)
		return (1);
	else if (access(full_path, F_OK) == 0)
	{
		*found = 1;
		return (0);
	}
	return (-1);
}