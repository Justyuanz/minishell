#include "vec.h"

int vec_push(t_vec *dst, char *element)
{
	int	i;

	i = 0;
	if (!dst || !element)
		return (-1);
	if (!dst->memory)
	{	
		if (vec_new(dst, 16) == -1)
			return (-1);
	}
	if (dst->len >= dst->capacity)
	{
		if (vec_resize(dst, dst->len * 2) == -1)
			return (-1);
	}
	dst->memory[dst->len] = element;
	dst->len += 1;
	return (1);
}
