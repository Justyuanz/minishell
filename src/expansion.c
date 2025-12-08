/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:08:06 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/08 11:36:58 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	exit_status(t_data *d, size_t *i, char *buf, size_t *off)
{
	int		code;
	char	*num;
	size_t	k;

	if (d->line[*i] == '?')
	{
		k = 0;
		code = ft_shell()->exitcode;
		num = ft_itoa(code);
		while (num[k])
			buf[(*off)++] = num[k++];
		free(num);
		(*i)++;
		return (true);
	}
	return (false);
}

void	handle_expansion(t_data *d, char *buf, size_t *i, size_t *off)
{
	(*i)++;
	if (exit_status(d, i, buf, off))
		return ;
	if (bare_dollar(d, buf, off, i))
		return ;
	handle_variable(d, buf, i, off);
}
