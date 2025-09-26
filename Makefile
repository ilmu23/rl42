## ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
## ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
## █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
## ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
## ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
## ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
##
## <<Makefile>>

NAME	=	librl42.a

BUILD	=	fsan

## COMPILER FLAGS

CC				=	gcc
cflags.common	=	-Wall -Wextra -Werror -Wpedantic -std=c2x -pedantic-errors -I$(INCDIR)
cflags.debug	=	-g -D__DEBUG_BUILD
cflags.fsan		=	$(cflags.debug) -fsanitize=address,undefined
cflags.normal	=	-s -O1
cflags.extra	=	
CFLAGS			=	$(cflags.common) $(cflags.$(BUILD)) $(cflags.extra)

## DIRECTORIES

BINDIR	=	bin
SRCDIR	=	src
OBJDIR	=	obj
INCDIR	=	inc
TESTDIR	=	tst

FUNCDIR	=	function
HISTDIR	=	history
KBINDIR	=	input
KEYBDIR	=	keybinds
TERMDIR	=	term
UTILDIR	=	utils

### RL42 FUNCTION DIRS

RLFNDIR	=	fn

INFNDIR	=	input
HSFNDIR	=	history

## SOURCE FILES

FUNCFILES	=	rl42_fn_info.c

HISTFILES	=	history.c

KBINFILES	=	listen.c

KEYBFILES	=	editing_mode.c \
				keyseq.c \
				rl42_bind.c

TERMFILES	=	cursor.c \
				display.c \
				settings.c

UTILFILES	=	list.c \
				map.c \
				message.c \
				rl42_string.c \
				strhash.c \
				utf8.c \
				vector.c

RLFNFILES	=	$(addprefix $(INFNDIR)/, $(INFNFILES)) \
				$(addprefix $(HSFNDIR)/, $(HSFNFILES))

INFNFILES	=	self_insert.c

HSFNFILES	=	accept_line.c

FILES	=	rl42.c \
			init.c \
			$(addprefix $(FUNCDIR)/, $(FUNCFILES)) \
			$(addprefix $(HISTDIR)/, $(HISTFILES)) \
			$(addprefix $(KBINDIR)/, $(KBINFILES)) \
			$(addprefix $(KEYBDIR)/, $(KEYBFILES)) \
			$(addprefix $(TERMDIR)/, $(TERMFILES)) \
			$(addprefix $(UTILDIR)/, $(UTILFILES)) \
			$(addprefix $(RLFNDIR)/, $(RLFNFILES))

SRCS	=	$(addprefix $(SRCDIR)/, $(FILES))
OBJS	=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

## TESTS

TCFLAGS	=	$(CFLAGS) -D__TEST_BUILD

TESTBIN		=	$(TESTDIR)/$(BINDIR)

### FUNCTION TESTS
FUNCTION_TEST		=	$(TESTBIN)/function_test

FUNCTION_TEST_FILES	=	$(TESTDIR)/$(FUNCDIR)/function_test.c \
						$(SRCS)

### HISTORY TESTS
HISTORY_TEST		=	$(TESTBIN)/history_test

HISTORY_TEST_FILES	=	$(TESTDIR)/$(HISTDIR)/history_test.c \
						$(SRCS)

### KEYBIND TESTS
KEYBIND_TEST		=	$(TESTBIN)/keybind_test

KEYBIND_TEST_FILES	=	$(TESTDIR)/$(KEYBDIR)/keybind_test.c \
						$(SRCS)

### UTIL TESTS
STRLEN_UTF8_TEST		=	$(TESTBIN)/strlen_utf8_test
RL42_STRING_TEST		=	$(TESTBIN)/rl42_string_test
VECTOR_TEST				=	$(TESTBIN)/vector_test
LIST_TEST				=	$(TESTBIN)/list_test
MAP_TEST				=	$(TESTBIN)/map_test

STRLEN_UTF8_TEST_FILES	=	$(TESTDIR)/$(UTILDIR)/strlen_utf8.c \
							$(SRCDIR)/$(UTILDIR)/rl42_string.c \
							$(SRCDIR)/$(UTILDIR)/vector.c \
							$(SRCDIR)/$(UTILDIR)/utf8.c

RL42_STRING_TEST_FILES	=	$(TESTDIR)/$(UTILDIR)/rl42_string.c \
							$(SRCDIR)/$(UTILDIR)/rl42_string.c \
							$(SRCDIR)/$(UTILDIR)/vector.c \
							$(SRCDIR)/$(UTILDIR)/utf8.c

VECTOR_TEST_FILES		=	$(TESTDIR)/$(UTILDIR)/vector.c \
							$(SRCDIR)/$(UTILDIR)/vector.c \
							$(SRCDIR)/$(UTILDIR)/message.c

LIST_TEST_FILES			=	$(TESTDIR)/$(UTILDIR)/list.c \
							$(SRCDIR)/$(UTILDIR)/list.c \
							$(SRCDIR)/$(UTILDIR)/vector.c \
							$(SRCDIR)/$(UTILDIR)/message.c

MAP_TEST_FILES			=	$(TESTDIR)/$(UTILDIR)/map.c \
							$(SRCDIR)/$(UTILDIR)/map.c \
							$(SRCDIR)/$(UTILDIR)/vector.c \
							$(SRCDIR)/$(UTILDIR)/strhash.c \
							$(SRCDIR)/$(UTILDIR)/message.c

### INTERACTIVE TESTER
INTERACTIVE_TESTER	=	$(TESTBIN)/interactive

ITBUILD	=	fsan

ITCFLAGS	=	$(cflags.common) $(cflags.$(ITBUILD)) $(cflags.extra)
ITLDFLAGS	=	-L. -lrl42

all: $(NAME)

tester: $(INTERACTIVE_TESTER)

$(NAME): $(OBJDIR) $(OBJS)
	@printf "\e[1;38;5;27mRL42 >\e[m Creating %s\n" $@
	@ar -crs $(NAME) $(OBJS)
	@printf "\e[1;38;5;27mRL42 >\e[m \e[1mDone!\e[m\n"

$(INTERACTIVE_TESTER):
	@make --no-print-directory re BUILD=$(ITBUILD)
	@make --no-print-directory $(TESTBIN)
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(ITCFLAGS) $(TESTDIR)/interactive/main.c $(ITLDFLAGS) -o $@
	@printf "\e[1;38;5;27mRL42 >\e[m \e[1mDone!\e[m\n"

tests: $(TESTDIR)/$(BINDIR) functests histtests keybtests utiltests
	@printf "\e[1;38;5;27mRL42 >\e[m All tests passed!\n"

functests: $(FUNCTION_TEST)
	@./run_test rl42_fn $(FUNCTION_TEST)
	@printf "\e[1;38;5;27mRL42 >\e[m All function tests passed!\n"

histtests: $(HISTORY_TEST)
	@./run_test history $(HISTORY_TEST)
	@printf "\e[1;38;5;27mRL42 >\e[m All history tests passed!\n"

keybtests: $(KEYBIND_TEST)
	@./run_test rl42_bind $(KEYBIND_TEST)
	@printf "\e[1;38;5;27mRL42 >\e[m All keybind tests passed!\n"

utiltests: $(STRLEN_UTF8_TEST) $(RL42_STRING_TEST) $(VECTOR_TEST) $(LIST_TEST) $(MAP_TEST)
	@./run_test strlen_utf8 $(STRLEN_UTF8_TEST)
	@./run_test rl42_string $(RL42_STRING_TEST)
	@./run_test vector $(VECTOR_TEST)
	@./run_test list $(LIST_TEST)
	@./run_test map $(MAP_TEST)
	@printf "\e[1;38;5;27mRL42 >\e[m All util tests passed!\n"

$(FUNCTION_TEST): $(FUNCTION_TEST_FILES)
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ -o $@

$(HISTORY_TEST): $(HISTORY_TEST_FILES)
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ -o $@

$(KEYBIND_TEST): $(KEYBIND_TEST_FILES)
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ -o $@

$(STRLEN_UTF8_TEST): $(STRLEN_UTF8_TEST_FILES)
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ -o $@

$(RL42_STRING_TEST): $(RL42_STRING_TEST_FILES)
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ -o $@

$(VECTOR_TEST): $(VECTOR_TEST_FILES)
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ -o $@

$(LIST_TEST): $(LIST_TEST_FILES)
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ -o $@

$(MAP_TEST): $(MAP_TEST_FILES)
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ -o $@

$(OBJDIR):
	@printf "\e[1;38;5;27mRL42 >\e[m Creating objdirs\n"
	@mkdir -p $(OBJDIR)/$(FUNCDIR)
	@mkdir -p $(OBJDIR)/$(HISTDIR)
	@mkdir -p $(OBJDIR)/$(KBINDIR)
	@mkdir -p $(OBJDIR)/$(KEYBDIR)
	@mkdir -p $(OBJDIR)/$(TERMDIR)
	@mkdir -p $(OBJDIR)/$(UTILDIR)
	@mkdir -p $(OBJDIR)/$(RLFNDIR)/$(INFNDIR)
	@mkdir -p $(OBJDIR)/$(RLFNDIR)/$(HSFNDIR)

$(TESTBIN):
	@printf "\e[1;38;5;27mRL42 >\e[m Creating test executable dir\n"
	@mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@printf "\e[1;38;5;27mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -f $(OBJS)

tclean:
	@rm -f $(FUNCTION_TEST)
	@rm -f $(HISTORY_TEST)
	@rm -f $(KEYBIND_TEST)
	@rm -f $(STRLEN_UTF8_TEST)
	@rm -f $(RL42_STRING_TEST)
	@rm -f $(VECTOR_TEST)
	@rm -f $(LIST_TEST)
	@rm -f $(MAP_TEST)

fclean: clean tclean
	@rm -rf $(TESTBIN)
	@rm -rf $(OBJDIR)
	@rm -f $(NAME)

re: fclean all

retest: tclean tests

db:
	@printf "\e[1;38;5;27mRL42 >\e[m Creating compilation command database\n"
	@compiledb make --no-print-directory BUILD=$(BUILD) cflags.extra=$(cflags.extra) | sed -E '/^##.*\.\.\.$$|^[[:space:]]*$$/d'
	@printf "\e[1;38;5;27mRL42 >\e[m \e[1mDone!\e[m\n"

.PHONY: all tests utiltests clean tclean fclean re retest db
