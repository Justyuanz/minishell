/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:37:12 by jinzhang          #+#    #+#             */
/*   Updated: 2025/04/30 18:47:43 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isset(char c, char const *set)
{
	size_t	i;

	i = 0;
	if (set != NULL)
	{
		while (set[i])
		{
			if (c == set[i])
				return (1);
			i++;
		}
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*sub;
	size_t	start;
	size_t	len;

	if (!s1)
		return (NULL);
	start = 0;
	len = ft_strlen(s1);
	while (s1[start] && ft_isset(s1[start], set))
		start++;
	while (len > start && s1[len - 1] && ft_isset(s1[len - 1], set))
		len--;
	sub = ft_substr(s1, start, (len - start));
	return (sub);
}
