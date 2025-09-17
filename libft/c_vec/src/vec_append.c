#include "vec.h"

int vec_append(t_vec *dst, t_vec *src)
{
	if(!dst || !src || !src->memory)
		return (-1);
	if (!dst->memory)
		vec_new(dst, 1);
	if (dst->capacity <= (dst->len + src->len))
	{
		if ((dst->len + src->len) < dst->len * 2)
		{
			if (vec_resize(dst, dst->len * 2) == -1)
				return (-1);
		}
		else
		{
			if (vec_resize(dst, dst->len + src->len) == -1)
				return (-1);
		}
	}
	ft_memcpy(&dst->memory[dst->len], src->memory, src->len);
	dst->len += src->len;
	return (1);
}
