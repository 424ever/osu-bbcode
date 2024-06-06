BUILDDIR ?= $(shell pwd)
BUILDDIR := $(abspath $(BUILDDIR))
SRCDIR    = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
CFLAGS   += -Wall -Wextra -Werror --pedantic-errors
CFLAGS   += -I$(SRCDIR)/src
CFLAGS   += -I$(SRCDIR)/include

SRCS      = $(SRCDIR)/src/error.c   \
            $(SRCDIR)/src/unicode.c \
            $(SRCDIR)/src/utf8.c
OBJS      = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))
DEPS      = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.d, $(SRCS))
LIB       = $(BUILDDIR)/libunicode.a
TESTS     = $(patsubst $(SRCDIR)/tests/%.c, $(BUILDDIR)/tests/%, $(wildcard $(SRCDIR)/tests/test_*.c))

$(LIB): $(OBJS)
	$(AR) rcs $@ $^ $(LDFLAGS)

$(BUILDDIR)/../libtest/libtest.a: force_look
	$(MAKE) -C $(SRCDIR)../libtest BUILDDIR=$(BUILDDIR)/../libtest 

define execute
$(1)

endef

$(BUILDDIR)/tests/test_%: $(SRCDIR)/tests/test_%.c $(SRCS) $(SRCDIR)/tests/common.c $(BUILDDIR)/../libtest/libtest.a
	@mkdir -p $(@D)
	$(CC) -Isrc -Iinclude -I../libtest/include -D_POSIX_C_SOURCE=200809L -o $@ $^

.PHONY: check
check: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, $(t)))

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(LIB)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(wildcard $(BUILDDIR)/tests/*[^ch])

.PHONY: veryclean
veryclean: clean
	$(RM) $(SRCDIR)/compile_commands.json

.PHONY: lsp
lsp: $(SRCDIR)/compile_commands.json

$(SRCDIR)/compile_commands.json: $(SRCS)
	bear -- make -B

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

%.d: %.c
	$(CC) -MM $(CFLAGS) $^ -MF $@

-include $(DEPS)

force_look:
	@true
