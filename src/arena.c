/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 18:49:41 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:23:09 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	*arena_alloc(t_arena *arena, size_t elem_size)
{
	char	*current_start;
	size_t	misalign;
	size_t	alignment;

	if (!arena || !arena->data || elem_size == 0)
		return (NULL); // maybe add safe exit here?
	if (arena->offset + elem_size > arena->capacity)
		return (NULL); // out of memory, perhaps link to next arena
	alignment = 8;
	misalign = arena->offset % alignment;
	if (misalign != 0)
	{
		arena->offset = arena->offset + (alignment - misalign);
	}
	current_start = &arena->data[arena->offset];
	arena->offset += elem_size;
	return (current_start);
}

char	*arena_push(t_arena *arena, char *s, size_t len)
{
	char	*current_start;

	if (!arena || !s)
		return (NULL);
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

void	arena_reset(t_arena *arena)
{
	arena->offset = 0;
}

void	arena_free(t_arena *arena)
{
	free(arena->data);
	arena->data = NULL;
	arena->capacity = 0;
	arena->offset = 0;
}
