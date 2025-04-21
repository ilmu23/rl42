## ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
## ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
## █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
## ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
## ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
## ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
##
## <<Makefile>>

NAME	=	librl42.a

BUILD	=	normal

CC				=	gcc
cflags.common	=	-Wall -Wextra -Werror -Wpedantic -std=c23 -pedantic-errors -I$(INCDIR)
cflags.debug	=	-g
cflags.fsan		=	$(cflags.debug) -fsanitize=address,undefined
cflags.normal	=	-O3
cflags.extra	=	
CFLAGS			=	$(cflags.common) $(cflags.$(BUILD)) $(cflags.extra)

HIST_FILE	=	

ifneq ("$(HIST_FILE)", "")
	CFLAGS +=	-D_FT_RL_HFILE+$(HIST_FILE)
endif

SRCDIR	=	src
OBJDIR	=	obj
INCDIR	=	inc

FILES	=	rl42.c

SRCS	=	$(addprefix $(SRCDIR)/, $(FILES))
OBJS	=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	@printf "\e[1;35mRL42 >\e[m Creating %s\n" $@
	@ar -crs $(NAME) $(OBJS)
	@printf "\e[1;35mRL42 >\e[m \e[1mDone!\e[m\n"

$(OBJDIR):
	@printf "\e[1;35mRL42 >\e[m Creating objdir\n"
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@printf "\e[1;35mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -rf $(OBJDIR)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 
