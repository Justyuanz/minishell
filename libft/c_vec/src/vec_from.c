#include "vec.h"

int vec_from(t_vec *dst, char *src, size_t len)
{
	if(!src)
		return (-1);
	int	i;

	i = 0;
	if (vec_new(dst, len) < 0)
	{
		return (-1);
	}
	else
	{
		ft_memcpy(dst->memory, src, dst->capacity);
		if (!dst->memory)
			return (-1);
	}
	dst->len = len;
	return (1);
}

