/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:32:25 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/03 23:15:16 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

void	signal_handler(int sig)
{
	g_signal = sig;
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		(ft_shell()->exitcode) = 130;
	}
}


void	set_prompt_signals(void)
{
	struct sigaction	act;

	g_signal = 0;
	rl_done = 0;
	rl_event_hook = NULL;
	if (sigemptyset(&act.sa_mask) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	act.sa_handler = signal_handler;
	act.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &act, NULL) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}

void	set_parent_wait_signals(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}

void	set_child_signals(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}

