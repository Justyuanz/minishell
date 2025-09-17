#include "vec.h"

int vec_pop(char *dst, t_vec *src)
{
	if (!dst || !src || !src->memory || src->len == 0)
		return (-1);
	ft_memcpy(dst, &src->memory[src->len-1], 1);
	src->len -= 1;
	return (1);
}
