#include "vec.h"

char *vec_get(t_vec *src, size_t index)
{
	if (!src || !src->memory || src->len == 0 || index >= src->len)
		return (NULL);
	return (src->memory[index]);
}
