# Основная директория проекта
SRC_DIR	=	./src/

# Директория LibFT
FT_DIR	=	./libft/
FT_LNK	=	-L $(FT_DIR) -l ft

# Директория ReadLine
RL_DIR	=	/Users/ztune/.brew/Cellar/readline/8.1.1
#RL_INC	=	-I $(RL_DIR)/include # Закоментировать для Linux
#RL_LIB	=	-L $(RL_DIR)/lib # Закоментировать для Linux

# Параметры сборки и удаления
HEAD_LH	=	./libft/libft.h
HEAD_LA	=	./libft/libft.a
HEAD_PH	=	./src/minishell.h
NAME	=	minishell
SRCS	=	main.c utils.c signals.c puts.c prints.c \
			lexer/lexer.c lexer/parser.c lexer/corrector.c lexer/destroy.c lexer/dollar.c lexer/dollar_utils.c \
			parser/parser.c parser/parser_1.c parser/parser_2.c parser/parser_3.c parser/parser_4.c parser/destroy.c \
			executor/executor_1.c executor/executor_2.c executor/executor_3.c executor/fork.c executor/heredoc.c executor/fork_utils.c executor/fork_utils_2.c \
			commands/cd.c \
			commands/prompt.c \
			commands/pwd.c \
			commands/echo.c \
			commands/env.c \
			commands/export.c commands/export_utils.c \
			commands/unset.c \
			commands/test.c \
			commands/exit.c

OBJS	=	$(addprefix $(SRC_DIR),$(SRCS:.c=.o))
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -g
RM 		= 	rm -f

all:		makelib makeprog

bonus:		makelib makeprog

makelib:	$(HEAD_LH)
			make -C $(FT_DIR)

makeprog:	$(HEAD_LA) $(HEAD_PH)
			make $(NAME)

$(SRC_DIR)%.o:$(SRC_DIR)%.c $(HEAD_PH)
			$(CC) $(CFLAGS) -I $(FT_DIR) -I $(SRC_DIR) $(RL_INC) -o $@ -c $<

$(NAME):	$(OBJS) $(HEAD_LA)
			$(CC) $(CFLAGS) $(RL_LIB) -lreadline -lhistory $(OBJS) $(FT_LNK) -lm -o $(NAME)

clean:
			${RM} $(OBJS)
			make -C $(FT_DIR) clean

fclean:
			${RM} $(OBJS)
			make -C $(FT_DIR) fclean
			${RM} $(NAME)

re:			fclean all

.PHONY:		all clean fclean re bonus
