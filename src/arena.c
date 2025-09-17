#include "minishell.h"

int	arena_init(t_arena *arena, size_t capacity)
{
	if (!arena)
		return (-1);
	arena->data = ft_calloc(1, capacity);
	if (!arena->data)
		return (-1);
	arena->capacity = capacity;
	arena->offset = 0;
	return (1);
}

char *arena_push(t_arena *arena, char *s)
{
	size_t	len;
	char	*current_start;
	if (!arena || !s)
		return (NULL);
	len = ft_strlen(s) + 1;
	if (!arena->data && arena_init(arena, len) == -1)
		return (NULL);
	if (arena->offset + len > arena->capacity)
	{
		printf("ARENA IS FULL FIX MEEE!");
		return (NULL);
	}
	current_start = &arena->data[arena->offset];
	ft_memcpy(&arena->data[arena->offset], s, len);
	arena->offset += len;
	return (current_start);
}
void arena_reset(t_arena *arena)
{
	arena->offset = 0;
}
void arena_free(t_arena *arena)
{
	free(arena->data);
	arena->data = NULL;
	arena->capacity = 0;
	arena->offset = 0;
}
