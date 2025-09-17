/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:13:17 by jinzhang          #+#    #+#             */
/*   Updated: 2025/04/28 19:46:08 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	p;
	char	result;

	p = (long)n;
	result = 0;
	if (p < 0)
	{
		p = -p;
		write(fd, "-", 1);
	}
	if (p > 9)
	{
		ft_putnbr_fd(p / 10, fd);
	}
	result = p % 10 + '0';
	write(fd, &result, 1);
}
