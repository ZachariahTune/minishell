NAME	=	libft.a
SRCS	=	ft_strlen.c ft_strcpy.c ft_strcmp.c ft_strdup.c ft_memset.c ft_memcpy.c ft_memjoin.c ft_split.c ft_gnl.c ft_arrayfree.c ft_free.c \
			ft_miniglob.c ft_miniglob_utils1.c ft_miniglob_utils2.c ft_miniglob_destroy.c \
			ft_strncmp.c ft_isalpa.c ft_itoa.c ft_is_qoutes.c ft_strchr.c

HEAD	=	libft.h

OBJS	=	$(SRCS:.c=.o)

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -g
RM 		= 	rm -f

all: 		$(NAME)

%.o:%.c		$(HEAD)
			$(CC) $(CFLAGS) -I . -o $@ -c $<

$(NAME):	$(HEAD) $(OBJS)
			ar rcs $(NAME) $(OBJS)
			ranlib $(NAME)

clean:
			${RM} $(OBJS)

fclean:		clean
			${RM} ${NAME}

re:			fclean $(NAME)

.PHONY:		all clean fclean re
