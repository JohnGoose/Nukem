# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/17 14:20:16 by vkuikka           #+#    #+#              #
#    Updated: 2022/02/08 21:35:28 by vkuikka          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRC = ft_realloc.c ft_putchar.c ft_add_str.c ft_error.c ft_arrdup.c get_next_line.c get_next_line_utils.c ft_quicksort.c ft_atoi_base.c ft_itoa_base.c ft_strsplit_int.c ft_abs.c ft_permutations.c ft_swap.c ft_new_int_arr.c ft_new_char_arr.c ft_factorial.c ft_putnbr_arr.c ft_putchar_arr.c ft_word_count.c ft_len_words.c ft_fill_array.c ft_iscntrl.c ft_islower.c ft_isspace.c ft_isupper.c ft_sqrt.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memccpy.c ft_memmove.c ft_memchr.c ft_memcmp.c ft_strlen.c ft_strdup.c ft_strcpy.c ft_strncpy.c ft_strcat.c ft_strncat.c ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strstr.c ft_strnstr.c ft_strcmp.c ft_strncmp.c ft_atoi.c ft_atof.c ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c ft_memalloc.c ft_memdel.c ft_strnew.c ft_strdel.c ft_strclr.c ft_striter.c ft_striteri.c ft_strmap.c ft_strmapi.c ft_strequ.c ft_strnequ.c ft_strsub.c ft_strjoin.c ft_strtrim.c ft_strsplit.c ft_itoa.c ft_putstr.c ft_putendl.c ft_putnbr.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_lstnew.c ft_lstdelone.c ft_lstdel.c ft_lstadd.c ft_lstiter.c ft_lstmap.c
OBJECTS = $(SRC:.c=.o)
INCLUDES = includes
.PHONY: clean fclean re all

all: $(NAME)

$(NAME):
	@gcc -c -Wall -Wextra -Werror $(SRC) -I $(INCLUDES)
	@ar rc $(NAME) $(OBJECTS)
	@ranlib $(NAME)
	@make -C ft_printf

clean:
	@rm -f $(OBJECTS)
	@make clean -C ft_printf

fclean: clean
	@rm -f $(NAME)
	@make fclean -C ft_printf

re: fclean
	@make all
