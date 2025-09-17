#include "minishell.h"

void vec_test(void);
void print_vec(t_vec *v);
int arena_vec_test_(void);
int main(void)
{

    //vec_test();
	//updated vec_new vec_push vec_get vec_resize to work with array of ptr
	arena_vec_test_();
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