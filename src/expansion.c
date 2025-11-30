/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:08:06 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/28 19:08:09 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_expansion(t_data *d, char *buf, size_t *i, size_t *off)
{
	(*i)++;
	if (exit_status(d, i, buf, off))
		return ;
	if (bare_dollar(d, buf, off, i))
		return ;
	handle_variable(d, buf, i, off);
}
