#include "vec.h"

void	vec_free(t_vec *src)
{
	free(src->memory);
	src->memory = NULL;
	src->capacity = 0;
	src->len = 0;
}

