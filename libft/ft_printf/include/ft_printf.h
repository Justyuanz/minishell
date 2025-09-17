/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 10:17:46 by jinzhang          #+#    #+#             */
/*   Updated: 2025/09/11 21:00:19 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include "libft.h"

int	ft_printf(const char *format, ...);
int	isset(char c);
int	loop_str(const char *s, va_list *ap);
int	find_specs(char c, va_list *ap);
int	ft_putstr(char *s);
int	ft_putnbr(int n);
int	ft_putbase(unsigned long num, unsigned long base, char *str);
int	ft_puthex(unsigned long hex);
int	ft_puthexup(unsigned long hex);
int	ft_putaddress(void *ptr);
int	ft_putuint(unsigned int n);

#endif
