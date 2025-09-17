#include "vec.h"

int vec_remove(t_vec *src, size_t index)
{
	if (!src || !src->memory || index >= src->len)
		return (-1);
	if (index == (src->len - 1))
	{
		src->len -= 1;
		return (1);
	}
	if (!ft_memmove(vec_get(src, index), vec_get(src, index + 1), src->len - index))
		return (-1);
	src->len -= 1;
	return (1);
}