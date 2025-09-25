#ifndef VEC_H
# define VEC_H

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "libft.h"

typedef struct s_quote
{
	bool saw_single_quote;
	bool saw_double_quote;
}	t_quote;

typedef struct s_vec
{
	void			**memory;
	size_t			capacity;
	size_t			len;
	size_t			size_element;
	t_quote			q;
}	t_vec;

int		vec_new(t_vec *dst, size_t init_len, size_t size_element);
int		vec_resize(t_vec *src, size_t target_size);
int		vec_push(t_vec *dst, void *element);
void 	*vec_get(t_vec *src, size_t index);
void	vec_free(t_vec *src);
void 	vec_reset(t_vec *vec);

#endif
