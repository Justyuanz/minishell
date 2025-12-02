/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:32:25 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/02 16:59:32 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int sig)
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

void	set_prompt_signals(void)
{
	struct sigaction	act;
	//fprintf(stderr, "installing PROMPT MODE handler\n");
	if (sigemptyset(&act.sa_mask) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	act.sa_handler = signal_handler;
	act.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &act, NULL) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	//else
	//	fprintf(stderr,"handling ctrl c\n");
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
	//else
	//	fprintf(stderr,"handling ctrl \\\n");
}

void	set_parent_wait_signals(void)
{
//	fprintf(stderr, "installing PARENT MODE handler\n");
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
//	else
//		fprintf(stderr,"ignoring ctrl c\n");
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
//	else
//		fprintf(stderr,"ignoring ctrl \\\n");
}

void	set_child_signals(void)
{
//	fprintf(stderr, "installing CHILD MODE handler (shell_sigint)\n");
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
//	else
//		fprintf(stderr,"ctrl c defalt behaviour on\n");
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
//	else
//		fprintf(stderr,"ctrl \\ defalt behaviour on\n");
}
