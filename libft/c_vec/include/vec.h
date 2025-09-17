#ifndef VEC_H
# define VEC_H

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "libft.h"

typedef struct s_vec
{
	char			**memory;
	size_t			capacity;
	size_t			len;
}	t_vec;

int		vec_new(t_vec *dst, size_t init_len);
int		vec_from(t_vec *dst, char *src, size_t len);
int		vec_resize(t_vec *src, size_t target_size);
int		vec_push(t_vec *dst, char *element);
int 	vec_pop(char *dst, t_vec *src);
int 	vec_insert(t_vec *dst, char *element, size_t index);
int 	vec_remove(t_vec *src, size_t index);
int 	vec_append(t_vec *dst, t_vec *src);
int 	vec_prepend(t_vec *dst, t_vec *src);
char 	*vec_get(t_vec *src, size_t index);
void	vec_free(t_vec *src);

#endif
