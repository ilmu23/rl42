# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/13 11:30:59 by ivalimak          #+#    #+#              #
#    Updated: 2024/05/26 03:25:06 by ivalimak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	librl42.a

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
LIBDIR	=	libft

LFT		=	$(LIBDIR)/libft.a
INC		=	-I$(INCDIR) -I$(LIBDIR)/$(INCDIR)

INITDIR	=	init
MAPDIR	=	maps
FNDIR	=	fns

INITFILES	=	ft_rl_init.c \
				ft_rl_initfuncs.c \
				ft_rl_initkeys.c

MAPFILES	=	ft_rl_keymap.c \
				ft_rl_keymap_utils.c

FNFILES		=	fn_move.c \
				fn_hist.c \
				fn_text.c \
				fn_comp.c \
				fn_misc.c

FILES	=	ft_readline.c \
			ft_rl_error.c \
			ft_rl_exec.c \
			$(addprefix $(INITDIR)/, $(INITFILES)) \
			$(addprefix $(MAPDIR)/, $(MAPFILES)) \
			$(addprefix $(FNDIR)/, $(FNFILES))

SRCS	=	$(addprefix $(SRCDIR)/, $(FILES))
OBJS	=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(LFT) $(OBJDIR) $(OBJS)
	@printf "\e[1;35mRL42 >\e[m Creating %s\n" $@
	@ar -crs $(NAME) $(OBJS)
	@printf "\e[1;35mRL42 >\e[m \e[1mDone!\e[m\n"

$(LFT):
	@make --no-print-directory -C $(LIBDIR) BUILD=$(BUILD)

$(OBJDIR):
	@printf "\e[1;35mRL42 >\e[m Creating objdirs\n"
	@mkdir -p $(OBJDIR)/$(INITDIR)
	@mkdir -p $(OBJDIR)/$(MAPDIR)
	@mkdir -p $(OBJDIR)/$(FNDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@printf "\e[1;35mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@make --no-print-directory -C $(LIBDIR) clean
	@rm -f $(OBJS)

fclean: clean
	@make --no-print-directory -C $(LIBDIR) fclean
	@rm -rf $(OBJDIR)
	@rm -f $(NAME)

re: fclean all
