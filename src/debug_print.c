/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:06:48 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:12:09 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_cmds(t_data *d)
{
	t_cmd *cmd;     // one command node(argv + redirs)
	t_redir *redir; // single redirection
	t_quote *q;     // quotes
	size_t i;       // command index
	size_t j;       // argv index
	size_t k;       // redir index
	i = 0;
	// loop through all parsed commands (one per pipe segment)
	while (i < d->vec_cmds.len)
	{
		cmd = get_cmd(d, i); // get current command from vec_cmds
		fprintf(stderr, "cmd[%zu]:\n", i);
		// Print argv for this command
		// cmd->argv is a NULL-terminated array of char*
		// Example: ["echo", "hi", NULL]
		j = 0;
		while (cmd->argv && cmd->argv[j])
		{
			fprintf(stderr, "  argv[%zu]: %s\n", j, cmd->argv[j]);
			q = get_quote(cmd, j);
			fprintf(stderr, "        quote: single=%d double=%d\n",
				q->single_ON, q->double_ON);
			j++;
		}
		// Print redirections belonging to this command
		// each t_redir contains:
		// type: (REDIR_IN, REDIR_OUT, APPEND, HEREDOC)
		// file: filename or limiter string
		k = 0;
while (k < cmd->redirs.len)
		{
			redir = get_redir(cmd, k); // helper to fetch one redir
			fprintf(stderr, "  redir[%zu]: type=%s file='%s'\n" "       ambiguous= %d redir quote: single= %d double= %d expand_in_heredoc= %d\n", k, tok_type(redir->type), redir->file, redir->is_ambiguous, redir->quote.single_ON, redir->quote.double_ON, expand_in_heredoc(redir));
			k++;
		}
		i++;
		fprintf(stderr,"--------------------------------------\n");
	}
	//read_env_example(d);
	printf("\n\n\n");
}

void	debug_print_tokens(t_data *d)
{
	t_token	*tok;
	size_t	j;

	j = 0;
	fprintf(stderr, "\n===============TOKEN INFO==============\n");
	fprintf(stderr, "vec_tok.len = %zu\n\n", d->vec_tok.len);
	while (j < d->vec_tok.len)
	{
		tok = get_tok(d, j);
		fprintf(stderr, "vec_tok.memory[%zu] -> tok =%p\n", j, (void *)tok);
		fprintf(stderr, "            tok->str  :  \"%s\"\n", tok->str);
		fprintf(stderr, "            tok->type :  %s\n", tok_type(tok->type));
		fprintf(stderr, "--------------------------------------\n");
		j++;
	}
	fprintf(stderr, "\n===============END INFO==============\n");
}
