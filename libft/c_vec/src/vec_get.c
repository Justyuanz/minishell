/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:36:38 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 15:36:40 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

void	*vec_get(t_vec *src, size_t index)
{
	if (!src || !src->memory || src->len == 0 || index >= src->len)
		return (NULL);
	return (src->memory[index]);
}
