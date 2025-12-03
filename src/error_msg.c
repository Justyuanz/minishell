/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:07:44 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/03 17:31:42 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_and_exit(t_data *d, char *msg, int exitcode)
{
	parse_error_msg(msg, NULL, exitcode);
	cleanup_shell(d);
	exit(ft_shell()->exitcode);
}

bool	parse_error_msg(char *msg, char *var, int exitcode)
{
	if (msg)
	{
		ft_putstr_fd(msg, 2);
		ft_putstr_fd("\n", 2);
	}
	if (var)
	{
		ft_putstr_fd(" `", 2);
		ft_putstr_fd(var, 2);
		ft_putstr_fd(" '", 2);
		ft_putstr_fd("\n", 2);
	}
	//printf("shell->exitcode: %d\n", ft_shell()->exitcode);
	ft_shell()->exitcode = exitcode;
	//printf("exitcode: %d\n", exitcode);
	//printf("shell->exitcode: %d\n", ft_shell()->exitcode);
	return (false);
}
