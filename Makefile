# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/13 11:30:59 by ivalimak          #+#    #+#              #
#    Updated: 2024/04/13 14:21:22 by ivalimak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft_readline.a

BUILD	=	normal

CC				=	cc
cflags.common	=	-Wall -Wextra -Werror
cflags.debug	=	-g
cflags.debugm	=	$(cflags.debug) -D DEBUG_MSG=1
cflags.asan		=	$(cflags.debug) -fsanitize=address
cflags.normal	=	-Ofast
cflags.extra	=	
CFLAGS			=	$(cflags.common) $(cflags.$(BUILD)) $(cflags.extra)

SRCDIR	=	src
OBJDIR	=	obj
INCDIR	=	inc

FILES	=	ft_readline.c \
			ft_rl_color.c \
			ft_rl_complete.c \
			ft_rl_complete2.c \
			ft_rl_complete3.c \
			ft_rl_complete_utils.c \
			ft_rl_cursor.c \
			ft_rl_debug_utils.c \
			ft_rl_exec.c \
			ft_rl_fn.c \
			ft_rl_fn2.c \
			ft_rl_fn3.c \
			ft_rl_fn4.c \
			ft_rl_fn5.c \
			ft_rl_history.c \
			ft_rl_history_file.c \
			ft_rl_history_search.c \
			ft_rl_history_utils.c \
			ft_rl_init.c \
			ft_rl_initfuncs.c \
			ft_rl_initkeys.c \
			ft_rl_input.c \
			ft_rl_input_utils.c \
			ft_rl_keymap.c \
			ft_rl_keymap_lists.c \
			ft_rl_keymap_utils.c \
			ft_rl_term_utils.c \
			ft_rl_utils.c \
			ft_rl_utils2.c \
			ft_rl_utils3.c \
			ft_rl_wildcard.c \
			ft_rl_wildcard_utils.c \
			ft_rl_word.c \
			ft_rl_word_replace.c

SRCS	=	$(addprefix $(SRCDIR)/, $(FILES))
OBJS	=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	@printf "\e[1;35mFT_RL >\e[m Creating %s\n" $@
	@ar -crs $(NAME) $(OBJS)
	@printf "\e[1;35mFT_RL >\e[m \e[1mDone!\e[m\n"

$(OBJDIR):
	@printf "\e[1;35mFT_RL >\e[m Creating objdir\n"
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@printf "\e[1;35mFT_RL >\e[m Compiling %s\n" $@
	@$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -rf $(OBJDIR)
	@rm -f $(NAME)

re: fclean all
