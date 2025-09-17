#include "minishell.h"

void vec_test(void);
void print_vec(t_vec *v);
int arena_vec_test_(void);

//updated vec_new vec_push vec_get vec_resize to work with array of ptr
//a func to init arena and vector
//a func to destroy
//a func to exit?
//a func for readline
//a func for bash delimiters
//tokenizerecho
void shell_init(t_arena *arena, t_vec *token)
{
	if (arena_init(arena, 10000) == -1)
		exit(EXIT_FAILURE);
	if (vec_new(token, 8) == -1)
	{
		arena_free(arena);
		exit(EXIT_FAILURE);
	}
}

void read_input(t_arena *arena, t_vec *token)
{
	char	*line;
	char	*arena_line;
	char	*vec_tok;
	line = readline("~/Minishell$");
	if (!line)
	{
		//call the destroy func
		exit(EXIT_FAILURE);
	}
	if (*line)
	{
		add_history(line);
		arena_line = arena_push(arena, line);
		vec_push(token, arena_line);
	}
	free(line);
	vec_tok = vec_get(token, 0);
	printf("%s\n", vec_tok);
	arena_reset(arena);
}
int main(void)
{

	t_arena arena;
	t_vec	token;

	shell_init(&arena, &token);
	while (1)
	{
		read_input(&arena, &token);
		arena_reset(&arena);
	}
	arena_free(&arena);
	vec_free(&token);
	//arena_vec_test_();
	return 0;
}

/*
t_data *get_data()
{
	static t_data datastruct;

	return &datastruct;
}
*/

/*
• Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
• Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
• Implement the following redirections:
◦ < should redirect input.
◦ > should redirect output.
◦ << should be given a delimiter, then read the input until a line containing the
delimiter is seen. However, it doesn’t have to update the history!
◦ >> should redirect output in append mode.
• Implement pipes (| character). The output of each command in the pipeline is
connected to the input of the next command via a pipe.
• Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.
• Handle $? which should expand to the exit status of the most recently executed
foreground pipeline.
*/