#include "minishell.h"

void vec_test(void);
void print_vec(t_vec *v);
int arena_vec_test_(void);

//updated vec_new vec_push vec_get vec_resize to work with array of ptr

//a func to destroy
//a func to exit?
//tokenizerecho


t_data *get_data(void)
{
	static t_data d;

	return &d;
}

bool ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

void shell_init(t_data *d)
{
	//perhaps more things to init here
	if (arena_init(&d->arena, 10000) == -1)
		exit(EXIT_FAILURE);
	if (vec_new(&d->vec_tok, sizeof(t_)) == -1)
	{
		arena_free(&d->arena);
		exit(EXIT_FAILURE);
	}
	d->q.single_ON = false;
	d->q.double_ON = false;

}
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

//' ' = super safe → everything is literal.
//" " = semi safe → things like $ and \" still work.
//You can’t mix ' inside '...'.
//You can put " " inside '...' and vice versa.
size_t 	read_quote(t_data *d, char *line, size_t i)
{
	if (line[i] == '>')
		//check if it is one or two bcs they function differently
	{
		if (line[i + 1] == '>')
		{
			//do somthing
			i+=2;
		}
		else
		{
			//do something
			i++;
		}
	}
	return (i);
}

// < file → read input from file instead of keyboard.

// > file → write output to file (overwrite).

// >> file → write output to file (append).

// << should be given a delimiter, then read the input until a line containing the delimiter is seen.
void read_operator(t_data *d, char *line, size_t *i)
{

}
void tokenizer(t_data *d, char *line)
{
	size_t	i;

	while (line[i])
	{
		if (ft_isspace(line[i]) && !d->q.single_ON && !d->q.double_ON)
			i++;
		if (line[i] == '>' || line[i] == '<' || line[i] == '|' || line[i] == '$' )
			read_operator(d, line, &i);
		// if (line[i] == '<')
		// {
		// 	if (line[i + 1] == '<')
		// 		//do somthing
		// 	else
		// 		//do something
		// }
		// if (line[i] == '|')
		// {
		// 	//The output of each command in the pipeline is connected to the input of the next command via a pipe.
		// }
		// if (line[i] == '$')
		// {
		// 	if (line[i + 1] = '?')
		// 		// Handle $? which should expand to the exit status of the most recently executed foreground pipeline.
		// 	else
		// 		// Handle environment variables ($ followed by a sequence of characters) which should expand to their values.
		// }

	}
}

void read_input(t_data *d)
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
		//tokenize
		arena_line = arena_push(&d->arena, line);
		vec_push(&d->vec_tok, arena_line);
	}
	free(line);
	vec_tok = vec_get(&d->vec_tok, 0);
	printf("%s\n", vec_tok); //delete me
	arena_reset(&d->arena);
}
int main(void)
{
	t_data	*d;
	t_token	token;

	d = get_data();
	shell_init(d);
	while (1)
	{
		read_input(d);
	}
	arena_free(&d->arena);
	vec_free(&d->vec_tok);
	//arena_vec_test_();
	return 0;
}


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