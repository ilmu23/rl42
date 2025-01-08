# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/13 11:30:59 by ivalimak          #+#    #+#              #
#    Updated: 2025/01/08 21:32:59 by ivalimak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	librl42.a
OUTDIR	=	$(dir $(NAME))

BUILD	=	normal

CC				=	cc
cflags.common	=	-Wall -Wextra -Werror
cflags.debug	=	-g
cflags.debugm	=	$(cflags.debug) -D DEBUG_MSG=1
cflags.asan		=	$(cflags.debug) -fsanitize=address
cflags.normal	=	-Ofast
cflags.extra	=	
CFLAGS			=	$(cflags.common) $(cflags.$(BUILD)) $(cflags.extra)

HIST_FILE	=	

ifneq ("$(HIST_FILE)", "")
	CFLAGS +=	-D_FT_RL_HFILE+$(HIST_FILE)
endif

SRCDIR	=	src
OBJDIR	=	obj
LIBDIR	=	lib
INCDIR	=	inc

ifeq ($(strip $(OUTDIR)), "./")
	TI42	=	$(LIBDIR)/libti42.a
else
	TI42	=	$(OUTDIR)/libti42.a
endif
TI42DIR	=	$(LIBDIR)/ti42

INC		=	-I$(INCDIR) -I$(TI42DIR)/$(INCDIR)

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
				hlcolor.c \
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
				initfuncs.c

INPUTFILES	=	add.c \
				kill.c \
				rm.c \
				utils.c

KEYFILES	=	bind.c \
				fn.c \
				utils.c

SETTINGSFILES	=	utils.c

TERMFILES	=	cursor.c \
				utils.c

LFTFILES	=	__alloc.c \
				__calloc.c \
				__cast.c \
				__clean.c \
				__dprintf.c \
				__exit.c \
				__expand.c \
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
				__parse.c \
				__pop.c \
				__printf.c \
				__push.c \
				__pushtrap.c \
				__put.c \
				__return.c \
				__sprintf.c \
				__snprintf.c \
				__str.c \
				__sweep.c \
				__utils.c \
				__vm.c \
				__vprintf.c \
				__vdprintf.c \
				__vsprintf.c \
				__vsnprintf.c

FILES	=	rl42.c \
			color.c \
			error.c \
			kring.c \
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
	@$(CC) $(CFLAGS) $(INC) test.c -L. -L$(LIBDIR) -lrl42 -lti42 -lm -o $@
	@printf "\e[1;35mRL42 >\e[m \e[1mDone!\e[m\n"

$(NAME): $(TI42) $(OBJDIR) $(OBJS)
	@printf "\e[1;35mRL42 >\e[m Creating %s\n" $@
	@ar -crs $(NAME) $(OBJS)
	@printf "\e[1;35mRL42 >\e[m \e[1mDone!\e[m\n"

$(TI42):
	@git submodule init $(TI42DIR)
	@git submodule update $(TI42DIR)
	@make --no-print-directory -C $(TI42DIR) BUILD=$(BUILD) NAME=libti42.a
	@mv $(TI42DIR)/libti42.a $(TI42)

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
	@make --no-print-directory -C $(TI42DIR) clean NAME=libti42.a
	@rm -f $(OBJS)

fclean: clean
	@make --no-print-directory -C $(TI42DIR) fclean NAME=libti42.a
	@rm -rf $(OBJDIR)
	@rm -rf a.out.dSYM
	@rm -f a.out
	@rm -f $(NAME)
	@rm -f $(TI42)

re: fclean all

.PHONY: all test clean fclean re 
