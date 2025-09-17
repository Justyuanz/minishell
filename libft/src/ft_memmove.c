/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:43:46 by jinzhang          #+#    #+#             */
/*   Updated: 2025/04/29 16:19:09 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*out;
	char	*in;
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	out = (char *)dest;
	in = (char *)src;
	if (dest <= src)
	{
		while (i < n)
		{
			out[i] = in[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i--)
			out[i] = in[i];
	}
	return (dest);
}
