CFLAGS += -std=c89
CFLAGS += -Wall -Wextra -Werror --pedantic-errors
CFLAGS += -Isrc
CFLAGS += -Iinclude

SRCS    = src/error.c   \
          src/unicode.c \
          src/utf8.c
OBJS    = $(SRCS:.c=.o)
DEPS    = $(SRCS:.c=.d)
LIB     = libunicode.a
TESTS   = $(patsubst tests/%.c, tests/%, $(wildcard tests/test_*.c))

$(LIB): $(OBJS)
	$(AR) rcs $@ $^ $(LDFLAGS)

../libtest/libtest.a: force_look
	$(MAKE) -C ../libtest

define execute
$(1)

endef

tests/test_%: tests/test_%.c $(SRCS) tests/common.c ../libtest/libtest.a
	$(CC) -Isrc -Iinclude -I../libtest/include -std=c89 -o $@ $^

.PHONY: check
check: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, ./$(t)))

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(LIB)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(wildcard tests/*[^ch])

.PHONY: veryclean
veryclean: clean
	$(RM) compile_commands.json

.PHONY: lsp
lsp: compile_commands.json

compile_commands.json: $(SRCS)
	bear -- make -B

%.d: %.c
	$(CC) -MM $(CFLAGS) $^ -MF $@

-include $(DEPS)

force_look:
	@true
