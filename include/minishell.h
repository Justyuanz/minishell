#ifndef MINISHELL_H
# define MINISHELL_H

#include "vec.h"
#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
}	s_token_type;

typedef struct s_arena
{
	char	*data;
	size_t	capacity;
	size_t	offset;
}	t_arena;

int	arena_init(t_arena *arena, size_t capacity);
char *arena_push(t_arena *arena, char *s);
void arena_reset(t_arena *arena);
void arena_free(t_arena *arena);
#endif