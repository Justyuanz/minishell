GREEN = \033[0;92m

NAME = minishell
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp  --trace-children=yes --track-fds=yes

READLINE_INC = -I/opt/homebrew/opt/readline/include
READLINE_LIB = -L/opt/homebrew/opt/readline/lib

DFLAGS = -MMD -MP \
		-I include \
		-I libft/include \
		-I libft/ft_printf/include \
		-I libft/get_next_line/include \
		-I libft/c_vec/include \
		$(READLINE_INC) \

SRCDIR = src
OBJDIR = obj
LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

SRCS = $(addprefix $(SRCDIR)/,\
		main.c \
		tokenizer.c \
		check_syntax.c \
		arena.c \
		read_operators.c \
		read_word.c \
		read_word_utils.c \
		env.c \
		expansion.c \
		expansion_helpers.c \
		build_cmds.c \
		build_cmds_helpers.c \
		get.c \
		utils.c \
		error_msg.c \
		builtin/builtin.c builtin/builtin_echo.c execution/execution.c execution/execution_fork.c execution/single_command.c \
		execution/utils.c builtin/builtin_others.c builtin/builtin_cd.c execution/path1.c execution/path.c execution/checkers.c execution/cleaners.c \
		execution/piping.c execution/redirection.c execution/redir_child.c heredoc/heredoc.c heredoc/heredoc_input.c builtin/builtin_exit.c builtin/builtin_export.c \
		execution/envp.c signals/signals.c signals/heredoc_signals.c \
		cleanup.c builtin/export_utils.c\
)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(LIBFT_A):
		make -s -C $(LIBFT_DIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
		mkdir -p $(@D)
		@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT_A) $(C_VEC_A)
		@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(READLINE_LIB) -lreadline -o $(NAME)
		@echo "$(GREEN) Compiled!$(DEF_COLOR)"

valgrind: all
		$(VALGRIND) ./minishell

clean:
		rm -rf $(OBJDIR)
		make clean -s -C $(LIBFT_DIR)

fclean: clean
		rm -rf $(NAME)
		make fclean -s -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re valgrind
.SECONDARY:$(OBJS)
-include $(DEPS)