BUILDDIR     ?= $(shell pwd)
REAL_BUILDDIR = $(abspath $(BUILDDIR))
SRCDIR        = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
CFLAGS       += -Wall -Wextra -Werror --pedantic-errors
CFLAGS       += -I$(SRCDIR)/include

SRCS          = $(SRCDIR)/src/test.c
OBJS          = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.o, $(SRCS))
DEPS          = $(patsubst $(SRCDIR)/%.c, $(REAL_BUILDDIR)/%.d, $(SRCS))
LIB           = $(REAL_BUILDDIR)/libtest.a

$(LIB): $(OBJS)
	$(AR) rcs $@ $^ $(LDFLAGS)

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(LIB)
	$(RM) $(OBJS)
	$(RM) $(DEPS)

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
	$(CC) -MM $(CFLAGS) $^

-include $(DEPS)
