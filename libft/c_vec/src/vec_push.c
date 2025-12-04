/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_push.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:36:49 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 15:36:51 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

int	vec_push(t_vec *dst, void *element)
{
	int	i;

	i = 0;
	if (!dst)
		return (-1);
	if (!dst->memory)
	{
		if (vec_new(dst, 1, dst->size_element) == -1)
			return (-1);
	}
	if (dst->len * dst->size_element >= dst->capacity)
	{
		if (vec_resize(dst, dst->len * 2) == -1)
			return (-1);
	}
	dst->memory[dst->len] = element;
	dst->len += 1;
	return (1);
}
