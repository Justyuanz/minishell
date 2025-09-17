/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:05:02 by jinzhang          #+#    #+#             */
/*   Updated: 2025/04/29 16:19:57 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmem, size_t size)
{
	size_t			i;
	size_t			totalbytes;
	unsigned char	*ptr;

	i = 0;
	if (nmem == 0 || size == 0)
	{
		ptr = malloc(1);
		if (ptr == NULL)
			return (NULL);
		return ((void *)ptr);
	}
	if (size > SIZE_MAX / nmem)
		return (NULL);
	totalbytes = nmem * size;
	ptr = malloc(totalbytes);
	if (ptr == NULL)
		return (NULL);
	while (i < totalbytes)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}
