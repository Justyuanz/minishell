GREEN = \033[0;99m

NAME = libft.a
COMPILER = cc
COMPILERFLAGS = -g -Wall -Wextra -Werror
DFLAGS = -MMD -MP \
		-I include \
		-I ft_printf/include \
		-I get_next_line/include \
		-I c_vec/include \

SRCDIR = src
OBJDIR = obj
PRINTF_DIR = ft_printf
GNL_DIR = get_next_line
C_VEC_DIR = c_vec

LIB_SRCS = $(addprefix $(SRCDIR)/, \
		ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
		ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
		ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
		ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c \
		ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c \
		ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
		ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_strrchr.c \
)
GNL_SRCS = $(addprefix $(GNL_DIR)/src/, \
		get_next_line_bonus.c get_next_line_utils_bonus.c \
)
PRINTF_SRCS = $(addprefix $(PRINTF_DIR)/src/, \
		ft_printf.c ft_put.c ft_basenbr.c \
)

C_VEC_SRCS = $(addprefix $(C_VEC_DIR)/src/,\
	vec_free.c vec_from.c vec_new.c vec_resize.c vec_push.c \
	vec_pop.c vec_get.c vec_insert.c vec_remove.c vec_append.c vec_prepend.c \
)

SRCS = $ $(LIB_SRCS) $(GNL_SRCS) $(PRINTF_SRCS) $(C_VEC_SRCS)
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN) libft built $(DEF_COLOR)"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY:$(OBJS)
-include $(DEPS)
