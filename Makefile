GENINFO       ?= geninfo
GENHTML       ?= genhtml

BUILDDIR      ?= $(shell pwd)
REAL_BUILDDIR  = $(abspath $(BUILDDIR))
SRCDIR         = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
CFLAGS        += -D_DEFAUT_SOURCE
CFLAGS        += -Wall -Wextra -Werror --pedantic-errors
CFLAGS        += -I$(SRCDIR)/src

SRCS           = $(SRCDIR)/src/alloc.c   \
                 $(SRCDIR)/src/bbcode.c  \
		 $(SRCDIR)/src/error.c   \
		 $(SRCDIR)/src/free.c    \
                 $(SRCDIR)/src/main.c    \
                 $(SRCDIR)/src/parse.c   \
                 $(SRCDIR)/src/unicode.c \
                 $(SRCDIR)/src/utf8.c
OBJS           = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.o, $(SRCS))
DEPS           = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.d, $(SRCS))
BIN            = $(REAL_BUILDDIR)/osu-bbcode
TESTS          = $(patsubst $(SRCDIR)/t/test-%.c, $(REAL_BUILDDIR)/t/%.test, $(wildcard $(SRCDIR)/t/test-*.c))

$(BIN): $(OBJS)
	@printf "%b" " \033[0;34mLD\t\033[0;35m$@\033[m\n"
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

define execute
$(1)

endef

define exec_valgrind
valgrind $(1)

endef

$(REAL_BUILDDIR)/t/%.test: $(SRCDIR)/t/test-%.c $(SRCS) $(SRCDIR)/libtap/tap.c
	@printf "%b" " \033[0;34mCC\t\033[0;36m$@\033[m\n"
	@mkdir -p $(@D)
	@$(CC) -DTEST -Og -g --coverage -I$(SRCDIR)/src -I$(SRCDIR)/libtap -o $@ $^

.PHONY: check
check: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, $(t)))

.PHONY: memcheck
memcheck: $(TESTS)
	$(foreach t, $(TESTS), $(call exec_valgrind, $(t)))

.PHONY: coverage
coverage: $(TESTS)
	@$(foreach t, $(TESTS), $(call execute, $(t)))
	@printf "%b" " \033[0;34mGENINFO\t\033[0;36m$(REAL_BUILDDIR)/coverage.info\033[m\n"
	@$(GENINFO) $(REAL_BUILDDIR)/t -b $(SRCDIR) -o $(REAL_BUILDDIR)/coverage.info
	@printf "%b" " \033[0;34mGENHTML\t\033[0;36m$(REAL_BUILDDIR)/coverage\033[m\n"
	@$(GENHTML) $(REAL_BUILDDIR)/coverage.info -o $(REAL_BUILDDIR)/coverage

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	@$(RM) $(BIN)
	@$(RM) $(OBJS)
	@$(RM) $(DEPS)
	@$(RM) $(TESTS)
	@$(RM) $(REAL_BUILDDIR)/t/*.gcda
	@$(RM) $(REAL_BUILDDIR)/t/*.gcno
	@$(RM) $(REAL_BUILDDIR)/coverage.info
	@$(RM) -r $(REAL_BUILDDIR)/coverage

.PHONY: veryclean
veryclean: clean
	@$(RM) compile_commands.json

.PHONY: lsp
lsp: $(SRCDIR)/compile_commands.json

$(SRCDIR)/compile_commands.json: $(SRCS)
	@bear -- make -B

$(REAL_BUILDDIR)/%.o: $(SRCDIR)/%.c
	@printf "%b" " \033[0;34mCC\t\033[0;36m$@\033[m\n"
	@mkdir -p $(@D)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

%.d: %.c
	@$(CC) -MM $(CFLAGS) $^ -MF $@

-include $(DEPS)

force_look:
	@true
