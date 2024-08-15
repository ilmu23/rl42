# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/13 11:30:59 by ivalimak          #+#    #+#              #
#    Updated: 2024/08/15 14:44:07 by ivalimak         ###   ########.fr        #
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

INC		=	-I$(INCDIR)

COMPDIR		=	comp
FNDIR		=	fns
HISTDIR		=	hist
INITDIR		=	init
INPUTDIR	=	input
KEYDIR		=	keys
SETTINGSDIR	=	settings
TERMDIR		=	term
LFTDIR		=	__lft

COMPFILES	=	completion.c \
				utils.c

FNFILES		=	move.c \
				hist.c \
				text.c \
				comp.c \
				misc.c \
				vi.c

HISTFILES	=	history.c \
				historyfile.c \
				search.c \
				utils.c

INITFILES	=	init.c \
				initfile.c \
				initfuncs.c \
				initkeys.c

INPUTFILES	=	add.c \
				kill.c \
				rm.c \
				utils.c

KEYFILES	=	keymap.c \
				utils.c

SETTINGSFILES	=	utils.c

TERMFILES	=	cursor.c \
				utils.c

LFTFILES	=	__alloc.c \
				__calloc.c \
				__clean.c \
				__exit.c \
				__getblksize.c \
				__lstadd_back.c \
				__lstadd_front.c \
				__lstfirst.c \
				__lstlast.c \
				__lstnew.c \
				__lstpop.c \
				__lstpopall.c \
				__lstpush.c \
				__lstpushall.c \
				__lstrmnode.c \
				__map_utils.c \
				__mapadd.c \
				__mapget.c \
				__mapnew.c \
				__mappop.c \
				__maprm.c \
				__mark.c \
				__misc.c \
				__obj.c \
				__objmap.c \
				__objpair.c \
				__pop.c \
				__push.c \
				__pushtrap.c \
				__put.c \
				__return.c \
				__str.c \
				__sweep.c \
				__vm.c

FILES	=	rl42.c \
			color.c \
			error.c \
			exec.c \
			utils.c \
			$(addprefix $(COMPDIR)/, $(COMPFILES)) \
			$(addprefix $(FNDIR)/, $(FNFILES)) \
			$(addprefix $(HISTDIR)/, $(HISTFILES)) \
			$(addprefix $(INITDIR)/, $(INITFILES)) \
			$(addprefix $(INPUTDIR)/, $(INPUTFILES)) \
			$(addprefix $(KEYDIR)/, $(KEYFILES)) \
			$(addprefix $(SETTINGSDIR)/, $(SETTINGSFILES)) \
			$(addprefix $(TERMDIR)/, $(TERMFILES)) \
			$(addprefix $(LFTDIR)/, $(LFTFILES))

SRCS	=	$(addprefix $(SRCDIR)/, $(FILES))
OBJS	=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: $(NAME)

test: a.out

a.out: $(NAME)
	@printf "\e[1;35mRL42 >\e[m Compiling test\n"
	@$(CC) $(CFLAGS) $(INC) test.c -L. -lrl42 -lm -o $@
	@printf "\e[1;35mRL42 >\e[m \e[1mDone!\e[m\n"

$(NAME): $(OBJDIR) $(OBJS)
	@printf "\e[1;35mRL42 >\e[m Creating %s\n" $@
	@ar -crs $(NAME) $(OBJS)
	@printf "\e[1;35mRL42 >\e[m \e[1mDone!\e[m\n"

$(LFT):
	@make --no-print-directory -C $(LIBDIR) BUILD=$(BUILD)

$(OBJDIR):
	@printf "\e[1;35mRL42 >\e[m Creating objdirs\n"
	@mkdir -p $(OBJDIR)/$(COMPDIR)
	@mkdir -p $(OBJDIR)/$(FNDIR)
	@mkdir -p $(OBJDIR)/$(HISTDIR)
	@mkdir -p $(OBJDIR)/$(INITDIR)
	@mkdir -p $(OBJDIR)/$(INPUTDIR)
	@mkdir -p $(OBJDIR)/$(KEYDIR)
	@mkdir -p $(OBJDIR)/$(SETTINGSDIR)
	@mkdir -p $(OBJDIR)/$(TERMDIR)
	@mkdir -p $(OBJDIR)/$(LFTDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@printf "\e[1;35mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
#	@make --no-print-directory -C $(LIBDIR) clean
	@rm -f $(OBJS)

fclean: clean
#	@make --no-print-directory -C $(LIBDIR) fclean
	@rm -rf $(OBJDIR)
	@rm -rf a.out.dSYM
	@rm -f a.out
	@rm -f $(NAME)

re: fclean all

.PHONY: all test clean fclean re
