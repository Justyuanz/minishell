/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_basenbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:58:24 by jinzhang          #+#    #+#             */
/*   Updated: 2025/05/09 16:16:54 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putbase(unsigned long num, unsigned long base, char *str)
{
	int	index;
	int	i;

	i = 0;
	if (num > (base - 1))
		i += ft_putbase(num / base, base, str);
	index = num % base;
	if (write(1, &str[index], 1) == -1)
		return (-1);
	i++;
	return (i);
}

int	ft_puthex(unsigned long hex)
{
	return (ft_putbase(hex, 16, "0123456789abcdef"));
}

int	ft_puthexup(unsigned long hex)
{
	return (ft_putbase(hex, 16, "0123456789ABCDEF"));
}

int	ft_putaddress(void *ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return (write(1, "(nil)", 5));
	if (write(1, "0x", 2) == -1)
		return (-1);
	i = ft_puthex((unsigned long)ptr) + 2;
	if (i == 1)
		return (-1);
	return (i);
}

int	ft_putuint(unsigned int u)
{
	return (ft_putbase(u, 10, "0123456789"));
}
