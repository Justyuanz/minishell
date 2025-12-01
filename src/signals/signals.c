/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:32:25 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:32:28 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		(ft_shell()->exitcode) = 130;
	}
}

void	shell_sigint(void)
{
	struct sigaction	act;

	if (sigemptyset(&act.sa_mask) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	act.sa_handler = ft_handle_sigint;
	act.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &act, NULL) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}

void	ft_sigignore(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}

void	ft_resetsignal(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}
