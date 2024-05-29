# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/13 11:30:59 by ivalimak          #+#    #+#              #
#    Updated: 2024/05/29 06:16:45 by ivalimak         ###   ########.fr        #
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

FNDIR		=	fns
HISTDIR		=	hist
INITDIR		=	init
INPUTDIR	=	input
KEYDIR		=	keys
TERMDIR		=	term

FNFILES		=	move.c \
				hist.c \
				text.c \
				comp.c \
				misc.c

HISTFILES	=	history.c \
				historyfile.c \
				utils.c

INITFILES	=	init.c \
				initfuncs.c \
				initkeys.c

INPUTFILES	=	utils.c

KEYFILES	=	keymap.c \
				utils.c

TERMFILES	=	cursor.c \
				utils.c

FILES	=	rl42.c \
			color.c \
			error.c \
			exec.c \
			$(addprefix $(FNDIR)/, $(FNFILES)) \
			$(addprefix $(HISTDIR)/, $(HISTFILES)) \
			$(addprefix $(INITDIR)/, $(INITFILES)) \
			$(addprefix $(INPUTDIR)/, $(INPUTFILES)) \
			$(addprefix $(KEYDIR)/, $(KEYFILES)) \
			$(addprefix $(TERMDIR)/, $(TERMFILES))

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
	@mkdir -p $(OBJDIR)/$(FNDIR)
	@mkdir -p $(OBJDIR)/$(HISTDIR)
	@mkdir -p $(OBJDIR)/$(INITDIR)
	@mkdir -p $(OBJDIR)/$(INPUTDIR)
	@mkdir -p $(OBJDIR)/$(KEYDIR)
	@mkdir -p $(OBJDIR)/$(TERMDIR)

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
