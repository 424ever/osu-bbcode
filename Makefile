BUILDDIR      ?= $(shell pwd)
REAL_BUILDDIR  = $(abspath $(BUILDDIR))
SRCDIR         = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
CFLAGS        += -D_POSIX_C_SOURCE=200809L
CFLAGS        += -Wall -Wextra -Werror --pedantic-errors
CFLAGS        += -I$(SRCDIR)/src
CFLAGS        += -I$(SRCDIR)/libunicode/include
LDFLAGS       += -L$(REAL_BUILDDIR)/libunicode -lunicode

SRCS           = $(SRCDIR)/src/free.c \
                 $(SRCDIR)/src/main.c \
                 $(SRCDIR)/src/parse.c
OBJS           = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.o, $(SRCS))
DEPS           = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.d, $(SRCS))
BIN            = $(REAL_BUILDDIR)/osu-bbcode
TESTS          = $(patsubst $(SRCDIR)/tests/%.c, $(REAL_BUILDDIR)/tests/%, $(wildcard $(SRCDIR)/tests/test_*.c))

LIBUNICODE_DIR = $(REAL_BUILDDIR)/libunicode
LIBUNICODE_LIB = $(LIBUNICODE_DIR)/libunicode.a
LIBTEST_DIR    = $(REAL_BUILDDIR)/libtest
LIBTEST_LIB    = $(LIBTEST_DIR)/libtest.a

$(BIN): $(OBJS) $(LIBUNICODE_LIB)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(LIBUNICODE_LIB): force_look
	$(MAKE) BUILDDIR=$(LIBUNICODE_DIR) -C $(SRCDIR)/libunicode

$(LIBTEST_LIB): force_look
	$(MAKE) BUILDDIR=$(LIBTEST_DIR) -C $(SRCDIR)/libtest

define execute
$(1)

endef

$(REAL_BUILDDIR)/tests/test_%: $(SRCDIR)/tests/test_%.c $(SRCS) $(LIBTEST_LIB)
	@mkdir -p $(@D)
	$(CC) -Isrc -Iinclude -o $@ $^

.PHONY: check
check: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, $(t)))
	$(MAKE) -C $(SRCDIR)/libunicode BUILDDIR=$(LIBUNICODE_DIR) check

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(MAKE) -C libunicode BUILDDIR=$(LIBUNICODE_DIR) clean
	$(MAKE) -C libtest BUILDDIR=$(LIBTEST_DIR) clean

.PHONY: veryclean
veryclean: clean
	$(RM) compile_commands.json
	$(MAKE) -C libunicode BUILDDIR=$(LIBUNICODE_DIR) veryclean
	$(MAKE) -C libtest BUILDDIR=$(LIBTEST_DIR) veryclean

.PHONY: lsp
lsp: $(SRCDIR)/compile_commands.json $(SRCDIR)/libunicode/compile_commands.json $(SRCDIR)/libtest/compile_commands.json

$(SRCDIR)/compile_commands.json: $(SRCS)
	bear -- make -B

$(SRCDIR)/libunicode/compile_commands.json: force_look
	$(MAKE) -C BUILDDIR=$(LIBUNICODE_DIR) libunicode lsp

$(SRCDIR)/libtest/compile_commands.json: force_look
	$(MAKE) -C BUILDDIR=$(LIBTEST_DIR) libtest lsp

$(REAL_BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

%.d: %.c
	$(CC) -MM $(CFLAGS) $^ -MF $@

-include $(DEPS)

force_look:
	@true
