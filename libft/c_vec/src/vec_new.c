/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_new.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:36:17 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 15:36:19 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

int	vec_new(t_vec *dst, size_t init_len, size_t size_element)
{
	if (!dst || size_element == 0)
		return (-1);
	dst->len = 0;
	dst->capacity = init_len * size_element;
	dst->size_element = size_element;
	if (init_len == 0)
	{
		dst->memory = NULL;
	}
	else
	{
		dst->memory = ft_calloc(1, dst->capacity);
		if (!dst->memory)
			return (-1);
	}
	return (1);
}
