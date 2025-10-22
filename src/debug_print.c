// #include "minishell.h"

// void debug_print_cmds(t_data *d)
// {
// 	size_t cmd_count;
// 	t_cmd	*cmd;

// 	cmd_count = get_cmd_count(d);
// 	fprintf(stderr,"cmd_count: %zu\n", cmd_count);
// 	fprintf(stderr, "\n===============CMD INFO==============\n");
//     for (size_t i = 0; i < cmd_count; i++)
//     {
//         cmd = get_cmd(d, i);
//         fprintf(stderr, "cmd[%zu]:\n", i);

//         for (size_t j = 0; cmd->argv[j]; j++)
//             fprintf(stderr, "  argv[%zu]: %s\n", j, cmd->argv[j]);

//         for (size_t k = 0; k < cmd->redirs.len; k++)
//         {
//             t_redir *r = (t_redir *)vec_get(&cmd->redirs, k);
//             fprintf(stderr, "  redir[%zu]: type=%s file=%s\n", k, tok_type(r->type), r->file);
//         }
// 		fprintf(stderr,"--------------------------------------\n");
//     }
// 	fprintf(stderr, "\n===============END INFO==============\n");
// }

// void debug_print_tokens(t_data *d)
// {
//     t_token *tok;
//     size_t  j;

//     j = 0;
//     fprintf(stderr, "\n===============TOKEN INFO==============\n");
//     fprintf(stderr,"vec_tok.len = %zu\n\n", d->vec_tok.len);
//     while (j < d->vec_tok.len)
//     {
//         tok = get_tok(d, j);
//         fprintf(stderr, "vec_tok.memory[%zu] -> tok =%p\n", j, (void *)tok);
//         fprintf(stderr,"            tok->str  :  \"%s\"\n",tok->str);
//         fprintf(stderr,"            tok->type :  %s\n", tok_type(tok->type));
//         fprintf(stderr,"--------------------------------------\n");
//         j++;
//     }
//     fprintf(stderr, "\n===============END INFO==============\n");
// }