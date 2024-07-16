GENINFO       ?= geninfo
GENHTML       ?= genhtml

BUILDDIR      ?= $(shell pwd)
REAL_BUILDDIR  = $(abspath $(BUILDDIR))
SRCDIR         = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
CFLAGS        += -D_POSIX_C_SOURCE=200809L
CFLAGS        += -Wall -Wextra -Werror --pedantic-errors -Wno-unused-function
CFLAGS        += -I$(SRCDIR)/src

SRCS           = $(SRCDIR)/src/alloc.c   \
		 $(SRCDIR)/src/error.c   \
		 $(SRCDIR)/src/free.c    \
                 $(SRCDIR)/src/main.c    \
                 $(SRCDIR)/src/parse.c   \
                 $(SRCDIR)/src/unicode.c \
                 $(SRCDIR)/src/utf8.c
OBJS           = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.o, $(SRCS))
DEPS           = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.d, $(SRCS))
BIN            = $(REAL_BUILDDIR)/osu-bbcode
TESTS          = $(patsubst $(SRCDIR)/tests/%.c, $(REAL_BUILDDIR)/tests/%, $(wildcard $(SRCDIR)/tests/test_*.c))

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

define execute
$(1)

endef

define exec_valgrind
valgrind $(1)

endef

$(REAL_BUILDDIR)/tests/test_%: $(SRCDIR)/tests/test_%.c $(SRCS) $(SRCDIR)/tests/test.c
	@mkdir -p $(@D)
	$(CC) -DTEST -Og --coverage -I$(SRCDIR)/src -o $@ $^

.PHONY: check
check: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, $(t)))

.PHONY: memcheck
memcheck: $(TESTS)
	$(foreach t, $(TESTS), $(call exec_valgrind, $(t)))

.PHONY: coverage
coverage: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, $(t)))
	$(GENINFO) $(REAL_BUILDDIR)/tests -b $(SRCDIR) -o $(REAL_BUILDDIR)/coverage.info
	$(GENHTML) $(REAL_BUILDDIR)/coverage.info -o $(REAL_BUILDDIR)/coverage

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(TESTS)
	$(RM) $(REAL_BUILDDIR)/tests/*.gcda
	$(RM) $(REAL_BUILDDIR)/tests/*.gcno
	$(RM) $(REAL_BUILDDIR)/coverage.info
	$(RM) -r $(REAL_BUILDDIR)/coverage

.PHONY: veryclean
veryclean: clean
	$(RM) compile_commands.json

.PHONY: lsp
lsp: $(SRCDIR)/compile_commands.json

$(SRCDIR)/compile_commands.json: $(SRCS)
	bear -- make -B

$(REAL_BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

%.d: %.c
	$(CC) -MM $(CFLAGS) $^ -MF $@

-include $(DEPS)

force_look:
	@true
