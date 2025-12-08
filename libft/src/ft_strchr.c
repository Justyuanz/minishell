/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:11:52 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/08 12:47:14 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	size_t	b;

	i = 0;
	b = ft_strlen(s);
	while (i <= b)
	{
		if ((unsigned char)c == s[i])
		{
			return ((char *)s + i);
		}
		i++;
	}
	return (NULL);
}
