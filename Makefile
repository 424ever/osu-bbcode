BUILDDIR ?= $(shell pwd)
BUILDDIR := $(abspath $(BUILDDIR))
SRCDIR    = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
CFLAGS   += -D_POSIX_C_SOURCE=200809L
CFLAGS   += -Wall -Wextra -Werror --pedantic-errors
CFLAGS   += -I$(SRCDIR)/src
CFLAGS   += -I$(SRCDIR)/libunicode/include
LDFLAGS  += -L$(BUILDDIR)/libunicode -lunicode

SRCS      = $(SRCDIR)/src/free.c \
            $(SRCDIR)/src/main.c \
            $(SRCDIR)/src/parse.c
OBJS      = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))
DEPS      = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.d, $(SRCS))
BIN       = $(BUILDDIR)/osu-bbcode
TESTS     = $(patsubst $(SRCDIR)/tests/%.c, $(BUILDDIR)/tests/%, $(wildcard $(SRCDIR)/tests/test_*.c))

$(BIN): $(OBJS) $(BUILDDIR)/libunicode/libunicode.a
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILDDIR)/libunicode/libunicode.a: force_look
	$(MAKE) BUILDDIR=$(BUILDDIR)/libunicode -C $(SRCDIR)/libunicode

$(BUILDDIR)/libtest/libtest.a: force_look
	$(MAKE) BUILDDIR=$(BUILDDIR)/libtest -C $(SRCDIR)/libtest

define execute
$(1)

endef

$(BUILDDIR)/tests/test_%: $(SRCDIR)/tests/test_%.c $(SRCS) $(BUILDDIR)/libtest/libtest.a
	@mkdir -p $(@D)
	$(CC) -Isrc -Iinclude -o $@ $^

.PHONY: check
check: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, $(t)))
	$(MAKE) -C $(SRCDIR)/libunicode BUILDDIR=$(BUILDDIR)/libunicode check

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(MAKE) -C libunicode BUILDDIR=$(BUILDDIR)/libunicode clean
	$(MAKE) -C libtest BUILDDIR=$(BUILDDIR)/libtest clean

.PHONY: veryclean
veryclean: clean
	$(RM) compile_commands.json
	$(MAKE) -C libunicode BUILDDIR=$(BUILDDIR)/libunicode veryclean
	$(MAKE) -C libtest BUILDDIR=$(BUILDDIR)/libtest veryclean

.PHONY: lsp
lsp: $(SRCDIR)/compile_commands.json $(SRCDIR)/libunicode/compile_commands.json $(SRCDIR)/libtest/compile_commands.json

$(SRCDIR)/compile_commands.json: $(SRCS)
	bear -- make -B

$(SRCDIR)/libunicode/compile_commands.json: force_look
	$(MAKE) -C BUILDDIR=$(BUILDDIR)/libunicode libunicode lsp

$(SRCDIR)/libtest/compile_commands.json: force_look
	$(MAKE) -C BUILDDIR=$(BUILDDIR)/libtest libtest lsp

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

%.d: %.c
	$(CC) -MM $(CFLAGS) $^ -MF $@

-include $(DEPS)

force_look:
	@true
