/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:35:32 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 15:35:35 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_H
# define VEC_H

# include "libft.h"
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_vec
{
	void	**memory;
	size_t	capacity;
	size_t	len;
	size_t	size_element;
}			t_vec;

int			vec_new(t_vec *dst, size_t init_len, size_t size_element);
int			vec_resize(t_vec *src, size_t target_size);
int			vec_push(t_vec *dst, void *element);
void		*vec_get(t_vec *src, size_t index);
void		vec_free(t_vec *src);
void		vec_reset(t_vec *vec);

#endif
