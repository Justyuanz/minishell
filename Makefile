GREEN = \033[0;92m

NAME = minishell
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp
DFLAGS = -MMD -MP \
		-I include \
		-I libft/include \
		-I libft/ft_printf/include \
		-I libft/get_next_line/include \
		-I libft/c_vec/include \

SRCDIR = src
OBJDIR = obj
LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

SRCS = $(addprefix $(SRCDIR)/,\
		main.c \
		tokenizer.c \
		debug_print.c \
		arena.c \
		read_operators.c \
		read_word.c \
		env.c \
		expansion.c \
		build_cmds.c \
		get.c \
		utils.c \
		executor.c \
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
		@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)
		@echo "$(GREEN) Compiled!$(DEF_COLOR)"

valgrind:
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