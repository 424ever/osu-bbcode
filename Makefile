CFLAGS  += -std=c89
CFLAGS  += -Wall -Wextra -Werror --pedantic-errors
CFLAGS  += -Isrc
CFLAGS  += -Ilibunicode/include
LDFLAGS += -Llibunicode -lunicode

SRCS     = src/free.c \
           src/main.c \
           src/parse.c
OBJS     = $(SRCS:.c=.o)
DEPS     = $(SRCS:.c=.d)
BIN      = osu-bbcode
TESTS    = $(patsubst tests/%.c, tests/%, $(wildcard tests/test_*.c))

$(BIN): $(OBJS) libunicode/libunicode.a
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

libunicode/libunicode.a: force_look
	$(MAKE) -C libunicode

libtest/libtest.a: force_look
	$(MAKE) -C libtest

define execute
$(1)

endef

tests/test_%: tests/test_%.c $(SRCS) libtest/libtest.a
	$(CC) -Isrc -Iinclude -std=c89 -o $@ $^

.PHONY: check
check: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, ./$(t)))
	$(MAKE) -C libunicode check

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(MAKE) -C libunicode clean
	$(MAKE) -C libtest clean

.PHONY: veryclean
veryclean: clean
	$(RM) compile_commands.json
	$(MAKE) -C libunicode veryclean
	$(MAKE) -C libtest veryclean

.PHONY: lsp
lsp: compile_commands.json libunicode/compile_commands.json libtest/compile_commands.json

compile_commands.json: $(SRCS)
	bear -- make -B

libunicode/compile_commands.json: force_look
	$(MAKE) -C libunicode lsp

libtest/compile_commands.json: force_look
	$(MAKE) -C libtest lsp

%.d: %.c
	$(CC) -MM $(CFLAGS) $^ -MF $@

-include $(DEPS)

force_look:
	true
