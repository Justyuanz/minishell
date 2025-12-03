#include"minishell.h"

void hd_signal_handler(int sig)
{
	g_signal = sig;
	if (sig == SIGINT)
		(ft_shell()->exitcode) = 130;
}

int	heredoc_event_hook(void)
{
	if (g_signal == SIGINT)
	{
		rl_done = 1;
		(ft_shell()->exitcode) = 130;
		return (1);
	}
	return (0);
}
void set_heredoc_signal(void)
{
	struct sigaction	sa;

	g_signal = 0;
	rl_done = 0;
	if (sigemptyset(&sa.sa_mask) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	rl_event_hook = heredoc_event_hook;
	sa.sa_handler = hd_signal_handler;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}