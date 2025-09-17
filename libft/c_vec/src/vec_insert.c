#include "vec.h"

int vec_insert(t_vec *dst, char *element, size_t index)
{
	if (!dst || !element || index > dst->len)
		return (-1);
	if (dst->capacity <= dst->len)
		if(vec_resize(dst, dst->len * 2) == -1)
			return (-1);
	if (index == dst->len)
		return (vec_push(dst, element));
	if (!ft_memmove(vec_get(dst, index + 1), vec_get(dst, index), dst->len - index))
		return (-1);
	if (!ft_memcpy(vec_get(dst, index), element, 1))
		return (-1);
	dst->len += 1;
	return (1);
}
