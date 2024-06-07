BUILDDIR     ?= $(shell pwd)
REAL_BUILDDIR = $(abspath $(BUILDDIR))
SRCDIR        = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
CFLAGS       += -Wall -Wextra -Werror --pedantic-errors
CFLAGS       += -I$(SRCDIR)/src
CFLAGS       += -I$(SRCDIR)/include

SRCS          = $(SRCDIR)/src/error.c   \
                $(SRCDIR)/src/unicode.c \
                $(SRCDIR)/src/utf8.c
OBJS          = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.o, $(SRCS))
DEPS          = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.d, $(SRCS))
LIB           = $(REAL_BUILDDIR)/libunicode.a
TESTS         = $(patsubst $(SRCDIR)/tests/%.c, $(REAL_BUILDDIR)/tests/%, $(wildcard $(SRCDIR)/tests/test_*.c))

$(LIB): $(OBJS)
	$(AR) rcs $@ $^ $(LDFLAGS)

$(REAL_BUILDDIR)/../libtest/libtest.a: force_look
	$(MAKE) -C $(SRCDIR)../libtest BUILDDIR=$(REAL_BUILDDIR)/../libtest 

define execute
$(1)

endef

$(REAL_BUILDDIR)/tests/test_%: $(SRCDIR)/tests/test_%.c $(SRCS) $(SRCDIR)/tests/common.c $(REAL_BUILDDIR)/../libtest/libtest.a
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
	$(RM) $(wildcard $(REAL_BUILDDIR)/tests/*[^ch])

.PHONY: veryclean
veryclean: clean
	$(RM) $(SRCDIR)/compile_commands.json

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
