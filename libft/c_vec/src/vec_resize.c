#include "vec.h"

int	vec_resize(t_vec *src, size_t target_len)
{
	t_vec	tmp;
	size_t	i;

	i = 0;
	if(!src)
		return (-1);
	if(!src->memory)
		return (vec_new(src, target_len, src->size_element));
	else
	{
		if (vec_new(&tmp, target_len, src->size_element) == -1)
			return (-1);
		else
		{
			while (i < src->len)
    		{
        		tmp.memory[i] = src->memory[i];
        		i++;
    		}
			tmp.len = src->len;
			vec_free(src);
			*src = tmp;
		}
	}
	return (1);
}
