/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:55:44 by jinzhang          #+#    #+#             */
/*   Updated: 2025/04/29 16:19:13 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*to;
	const char	*from;

	i = 0;
	if (!dest && !src)
		return (NULL);
	to = (char *)dest;
	from = (const char *)src;
	while (i < n)
	{
		to[i] = from[i];
		i++;
	}
	return (dest);
}
