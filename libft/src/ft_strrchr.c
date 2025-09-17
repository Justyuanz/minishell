/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:12:09 by jinzhang          #+#    #+#             */
/*   Updated: 2025/04/28 20:03:07 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	size_t	b;
	char	*j;

	i = 0;
	b = ft_strlen(s);
	j = NULL;
	while (i <= b)
	{
		if ((unsigned char)c == s[i])
		{
			j = (char *)s + i;
		}
		i++;
	}
	if (j == NULL)
		return (NULL);
	return (j);
}
