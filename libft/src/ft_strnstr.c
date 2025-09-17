/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:40:55 by jinzhang          #+#    #+#             */
/*   Updated: 2025/04/29 16:17:32 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	needlelen;

	j = 0;
	needlelen = ft_strlen(needle);
	if (needle[0] == 0)
		return ((char *)haystack);
	while (j < len && haystack[j] != '\0')
	{
		i = 0;
		while ((j + i) < len && needle[i] && needle[i] == haystack[j + i])
		{
			i++;
		}
		if (needle[i] == '\0')
		{
			return ((char *)&haystack[j]);
		}
		j++;
	}
	return (NULL);
}
