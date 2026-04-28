## ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
## ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
## █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
## ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
## ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
## ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
##
## <<Makefile>>

NAME	=	librl42
STATIC	=	$(NAME).a
SHARED	=	$(NAME).so

BUILD	=	NORMAL

## COMPILER FLAGS

CC				=	gcc
CFLAGS_COMMON	=	-Wall -Wextra -Werror -Wpedantic -std=gnu2x -pedantic-errors -fPIC -I$(INCDIR)
CFLAGS_DEBUG	=	-g -D__DEBUG_BUILD
CFLAGS_FSAN		=	$(CFLAGS_DEBUG) -fsanitize=address,undefined
CFLAGS_NORMAL	=	-s -O1
CFLAGS_EXTRA	?=	
CFLAGS			=	$(CFLAGS_COMMON) $(CFLAGS_$(BUILD)) $(CFLAGS_EXTRA)

ifdef USE_INTERNAL_CONTAINERS
	CFLAGS	+=	-D__RL42_USE_INTERNAL_CONTAINERS
endif

ifdef USE_INTERNAL_TERMINFO
	CFLAGS	+=	-D__RL42_USE_INTERNAL_TERMINFO
endif

ifndef NO_LIBICU
	CFLAGS	+=	-DUSE_LIBICU
endif

## LD FLAGS

LD				=	ld
LDFLAGS_EXTRA	?=	
LDFLAGS			=	-shared

## DIRECTORIES

BINDIR	=	bin
SRCDIR	=	src
OBJDIR	=	obj
INCDIR	=	inc
TESTDIR	=	tst

CONFDIR	=	config
FUNCDIR	=	function
HISTDIR	=	history
KBINDIR	=	input
KEYBDIR	=	keybinds
KILLDIR	=	kill
TERMDIR	=	term
UTILDIR	=	utils

INSTALL_PATH	?=	/usr

### RL42 FUNCTION DIRS

RLFNDIR	=	fn

HSFNDIR	=	history
KLFNDIR	=	kill
MCFNDIR	=	misc
MVFNDIR	=	move
TXFNDIR	=	text
VIFNDIR	=	vi

## SOURCE FILES

CONFFILES	=	load.c \
				settings.c

FUNCFILES	=	macro.c \
				rl42_fn_info.c

HISTFILES	=	history.c

KBINFILES	=	listen.c \
				match.c

KEYBFILES	=	editing_mode.c \
				keyseq.c \
				rl42_bind.c

KILLFILES	=	region.c \
				ring.c

TERMFILES	=	cursor.c \
				display.c \
				output.c \
				settings.c \
				utils.c

UTILFILES	=	completion.c \
				cstr_utils.c \
				message.c \
				misc.c \
				repeat.c \
				rl42_string.c \
				rl42str_utils.c \
				strhash.c \
				utf8.c \

ifdef USE_INTERNAL_CONTAINERS
	UTILFILES	+=	darray.c \
					list.c \
					map.c
endif

ifdef USE_INTERNAL_TERMINFO
	UTILFILES	+=	terminfo.c
endif

RLFNFILES	=	$(addprefix $(HSFNDIR)/, $(HSFNFILES)) \
				$(addprefix $(KLFNDIR)/, $(KLFNFILES)) \
				$(addprefix $(MCFNDIR)/, $(MCFNFILES)) \
				$(addprefix $(MVFNDIR)/, $(MVFNFILES)) \
				$(addprefix $(TXFNDIR)/, $(TXFNFILES)) \
				$(addprefix $(VIFNDIR)/, $(VIFNFILES))

HSFNFILES	=	accept_line.c \
				backward_history.c \
				backward_search_history.c \
				beginning_of_history.c \
				end_of_history.c \
				fetch_history.c \
				forward_history.c \
				forward_search_history.c \
				inc_backward_search_history.c \
				inc_forward_search_history.c \
				operate_and_get_next.c \
				yank_last_arg.c \
				yank_nth_arg.c

KLFNFILES	=	backward_copy_word.c \
				backward_kill_line.c \
				backward_kill_word.c \
				copy_region.c \
				copy_word.c \
				delete_horizontal_space.c \
				discard_line.c \
				forward_copy_word.c \
				kill_line.c \
				kill_region.c \
				kill_whole_line.c \
				kill_word.c \
				yank.c \
				yank_pop.c

MCFNFILES	=	abort.c \
				backward_char_search.c \
				dump_functions.c \
				dump_macros.c \
				dump_variables.c \
				emacs_editing_mode.c \
				exchange_point_and_mark.c \
				forward_char_search.c \
				numeric_argument.c \
				prefix_meta.c \
				reload_config_file.c \
				revert_line.c \
				set_mark.c \
				tilde_expand.c \
				unset_mark.c \
				vi_editing_mode.c

MVFNFILES	=	backward_char.c \
				backward_word.c \
				beginning_of_line.c \
				beginning_of_word.c \
				end_of_line.c \
				end_of_word.c \
				forward_char.c \
				forward_word.c

TXFNFILES	=	backward_delete_char.c \
				bracketed_paste.c \
				capitalize_word.c \
				clear_display.c \
				clear_screen.c \
				complete.c \
				delete_char.c \
				downcase_word.c \
				end_of_file.c \
				quoted_insert.c \
				self_insert.c \
				transpose_chars.c \
				transpose_words.c \
				upcase_word.c

VIFNFILES	=	vi_append.c \
				vi_append_eol.c \
				vi_command_mode.c \
				vi_delete.c \
				vi_insert.c \
				vi_insert_sol.c \
				vi_replace.c \
				vi_substitute.c \
				vi_substitute_line.c

FILES	=	rl42.c \
			init.c \
			$(addprefix $(CONFDIR)/, $(CONFFILES)) \
			$(addprefix $(FUNCDIR)/, $(FUNCFILES)) \
			$(addprefix $(HISTDIR)/, $(HISTFILES)) \
			$(addprefix $(KBINDIR)/, $(KBINFILES)) \
			$(addprefix $(KEYBDIR)/, $(KEYBFILES)) \
			$(addprefix $(KILLDIR)/, $(KILLFILES)) \
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
TERMINFO_TEST			=	$(TESTBIN)/terminfo_test
DARRAY_TEST				=	$(TESTBIN)/darray_test
LIST_TEST				=	$(TESTBIN)/list_test
MAP_TEST				=	$(TESTBIN)/map_test

STRLEN_UTF8_TEST_FILES	=	$(TESTDIR)/$(UTILDIR)/strlen_utf8.c \
							$(SRCDIR)/$(UTILDIR)/rl42_string.c \
							$(SRCDIR)/$(UTILDIR)/darray.c \
							$(SRCDIR)/$(UTILDIR)/utf8.c

RL42_STRING_TEST_FILES	=	$(TESTDIR)/$(UTILDIR)/rl42_string.c \
							$(SRCDIR)/$(UTILDIR)/rl42_string.c \
							$(SRCDIR)/$(UTILDIR)/darray.c \
							$(SRCDIR)/$(UTILDIR)/utf8.c

TERMINFO_TEST_FILES		=	$(TESTDIR)/$(UTILDIR)/terminfo.c \
							$(SRCDIR)/$(UTILDIR)/terminfo.c \
							$(SRCDIR)/$(UTILDIR)/cstr_utils.c \
							$(SRCDIR)/$(UTILDIR)/map.c \
							$(SRCDIR)/$(UTILDIR)/darray.c \
							$(SRCDIR)/$(UTILDIR)/strhash.c \
							$(SRCDIR)/$(UTILDIR)/message.c

DARRAY_TEST_FILES		=	$(TESTDIR)/$(UTILDIR)/darray.c \
							$(SRCDIR)/$(UTILDIR)/darray.c \
							$(SRCDIR)/$(UTILDIR)/message.c

LIST_TEST_FILES			=	$(TESTDIR)/$(UTILDIR)/list.c \
							$(SRCDIR)/$(UTILDIR)/list.c \
							$(SRCDIR)/$(UTILDIR)/darray.c \
							$(SRCDIR)/$(UTILDIR)/message.c

MAP_TEST_FILES			=	$(TESTDIR)/$(UTILDIR)/map.c \
							$(SRCDIR)/$(UTILDIR)/map.c \
							$(SRCDIR)/$(UTILDIR)/darray.c \
							$(SRCDIR)/$(UTILDIR)/strhash.c \
							$(SRCDIR)/$(UTILDIR)/message.c

### INTERACTIVE TESTER
INTERACTIVE_TESTER	=	$(TESTBIN)/interactive

ITBUILD	=	FSAN

ITCFLAGS	=	$(CFLAGS_COMMON) $(CFLAGS_$(ITBUILD)) $(CFLAGS_EXTRA)

ifeq ($(shell bash -c 'gcc -x c -<<< "#include <stdio.h> int main(void) { printf(\"%ld\n\", __STDC_VERSION__); }" && ./a.out && rm a.out'), 202311)
	TLDFLAGS	=	$(LDFLAGS_EXTRA)
	ITLDFLAGS	=	-L. -lrl42 $(TLDFLAGS)
else
	TLDFLAGS	=	$(LDFLAGS_EXTRA) -lbsd
	ITLDFLAGS	=	-L. -lrl42 $(TLDFLAGS)
endif

ifndef USE_INTERNAL_CONTAINERS
	TLDFLAGS	+=	-lcont
	ITLDFLAGS	+=	-lcont
endif

ifndef USE_INTERNAL_TERMINFO
	TLDFLAGS	+=	-lti42
	ITLDFLAGS	+=	-lti42
endif

ifndef NO_LIBICU
	TLDFLAGS	+=	-licuuc
	ITLDFLAGS	+=	-licuuc
endif

all: $(STATIC) $(SHARED)

static: $(STATIC)

shared: $(SHARED)

tester: $(INTERACTIVE_TESTER)

install:
	@printf "\e[1;38;5;39mRL42 >\e[m Installing rl42\n"
	@mkdir -p $(INSTALL_PATH)/lib
	@bash -c "if [ -a $(STATIC) ]; then cp $(STATIC) $(INSTALL_PATH)/lib/; fi"
	@bash -c "if [ -a $(SHARED) ]; then cp $(SHARED) $(INSTALL_PATH)/lib/; fi"
	@printf "\e[1;38;5;39mRL42 >\e[m \e[1mDone!\e[m\n"

install-headers:
	@printf "\e[1;38;5;39mRL42 >\e[m Installing headers\n"
	@mkdir -p $(INSTALL_PATH)/include/rl42
	@cp -r $(INCDIR)/* $(INSTALL_PATH)/include/rl42/
	@printf "\e[1;38;5;39mRL42 >\e[m \e[1mDone!\e[m\n"

$(STATIC): $(OBJDIR) $(OBJS)
	@printf "\e[1;38;5;39mRL42 >\e[m Creating %s\n" $@
	@ar -crs $@ $(OBJS)
	@printf "\e[1;38;5;39mRL42 >\e[m \e[1mDone!\e[m\n"

$(SHARED): $(OBJDIR) $(OBJS)
	@printf "\e[1;38;5;39mRL42 >\e[m Linking %s\n" $@
	@$(LD) $(LDFLAGS) $(OBJS) -o $@
	@printf "\e[1;38;5;39mRL42 >\e[m \e[1mDone!\e[m\n"

$(INTERACTIVE_TESTER): $(TESTBIN) $(SRCS) $(TESTDIR)/interactive/main.c
	@make --no-print-directory clean all BUILD=$(ITBUILD)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(ITCFLAGS) $(TESTDIR)/interactive/main.c $(ITLDFLAGS) -o $@
	@printf "\e[1;38;5;39mRL42 >\e[m \e[1mDone!\e[m\n"

tests: $(TESTDIR)/$(BINDIR) functests histtests keybtests utiltests
	@printf "\e[1;38;5;39mRL42 >\e[m All tests passed!\n"

functests: $(FUNCTION_TEST)
	@./run_test rl42_fn $(FUNCTION_TEST)
	@printf "\e[1;38;5;39mRL42 >\e[m All function tests passed!\n"

histtests: $(HISTORY_TEST)
	@./run_test history $(HISTORY_TEST)
	@printf "\e[1;38;5;39mRL42 >\e[m All history tests passed!\n"

keybtests: $(KEYBIND_TEST)
	@./run_test rl42_bind $(KEYBIND_TEST)
	@printf "\e[1;38;5;39mRL42 >\e[m All keybind tests passed!\n"

utiltests: $(STRLEN_UTF8_TEST) $(RL42_STRING_TEST) $(TERMINFO_TEST) $(DARRAY_TEST) $(LIST_TEST) $(MAP_TEST)
	@./run_test strlen_utf8 $(STRLEN_UTF8_TEST)
	@./run_test rl42_string $(RL42_STRING_TEST)
	@./run_test terminfo $(TERMINFO_TEST)
	@./run_test darray $(DARRAY_TEST)
	@./run_test list $(LIST_TEST)
	@./run_test map $(MAP_TEST)
	@printf "\e[1;38;5;39mRL42 >\e[m All util tests passed!\n"

$(FUNCTION_TEST): $(FUNCTION_TEST_FILES)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ $(TLDFLAGS) -o $@

$(HISTORY_TEST): $(HISTORY_TEST_FILES)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ $(TLDFLAGS) -o $@

$(KEYBIND_TEST): $(KEYBIND_TEST_FILES)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ $(TLDFLAGS) -o $@

$(STRLEN_UTF8_TEST): $(STRLEN_UTF8_TEST_FILES)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ $(TLDFLAGS) -o $@

$(RL42_STRING_TEST): $(RL42_STRING_TEST_FILES)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ $(TLDFLAGS) -o $@

$(TERMINFO_TEST): $(TERMINFO_TEST_FILES)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ $(TLDFLAGS) -o $@

$(DARRAY_TEST): $(DARRAY_TEST_FILES)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ $(TLDFLAGS) -o $@

$(LIST_TEST): $(LIST_TEST_FILES)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ $(TLDFLAGS) -o $@

$(MAP_TEST): $(MAP_TEST_FILES)
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(TCFLAGS) -I$(INCDIR) $^ $(TLDFLAGS) -o $@

$(OBJDIR):
	@printf "\e[1;38;5;39mRL42 >\e[m Creating objdirs\n"
	@mkdir -p $(OBJDIR)/$(CONFDIR)
	@mkdir -p $(OBJDIR)/$(FUNCDIR)
	@mkdir -p $(OBJDIR)/$(HISTDIR)
	@mkdir -p $(OBJDIR)/$(KBINDIR)
	@mkdir -p $(OBJDIR)/$(KEYBDIR)
	@mkdir -p $(OBJDIR)/$(KILLDIR)
	@mkdir -p $(OBJDIR)/$(TERMDIR)
	@mkdir -p $(OBJDIR)/$(UTILDIR)
	@mkdir -p $(OBJDIR)/$(RLFNDIR)/$(HSFNDIR)
	@mkdir -p $(OBJDIR)/$(RLFNDIR)/$(KLFNDIR)
	@mkdir -p $(OBJDIR)/$(RLFNDIR)/$(MCFNDIR)
	@mkdir -p $(OBJDIR)/$(RLFNDIR)/$(MVFNDIR)
	@mkdir -p $(OBJDIR)/$(RLFNDIR)/$(TXFNDIR)
	@mkdir -p $(OBJDIR)/$(RLFNDIR)/$(VIFNDIR)

$(TESTBIN):
	@printf "\e[1;38;5;39mRL42 >\e[m Creating test executable dir\n"
	@mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@printf "\e[1;38;5;39mRL42 >\e[m Compiling %s\n" $@
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

tclean:
	@rm -f $(INTERACTIVE_TESTER)
	@rm -f $(FUNCTION_TEST)
	@rm -f $(HISTORY_TEST)
	@rm -f $(KEYBIND_TEST)
	@rm -f $(STRLEN_UTF8_TEST)
	@rm -f $(RL42_STRING_TEST)
	@rm -f $(TERMINFO_TEST)
	@rm -f $(DARRAY_TEST)
	@rm -f $(LIST_TEST)
	@rm -f $(MAP_TEST)

fclean: clean tclean
	@rm -rf $(TESTBIN)
	@rm -rf $(OBJDIR)
	@rm -f $(STATIC)
	@rm -f $(SHARED)

re: fclean all

retest: tclean tests

db:
	@printf "\e[1;38;5;39mRL42 >\e[m Creating compilation command database\n"
	@compiledb make --no-print-directory BUILD=$(BUILD) cflags.extra="$(cflags.extra)" | sed -E '/^##.*\.\.\.$$|^[[:space:]]*$$/d'
	@printf "\e[1;38;5;39mRL42 >\e[m \e[1mDone!\e[m\n"

.PHONY: all tests utiltests clean tclean fclean re retest db
