/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:36:01 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 15:36:04 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec.h"

void	vec_free(t_vec *src)
{
	free(src->memory);
	src->memory = NULL;
	src->capacity = 0;
	src->len = 0;
	src->size_element = 0;
}
