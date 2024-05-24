CFLAGS  += -std=c89
CFLAGS  += -Wall -Wextra -Werror --pedantic-errors
CFLAGS  += -MMD
CFLAGS  += -Isrc
CFLAGS  += -Ilibunicode/include
CFLAGS  += -D_POSIX_C_SOURCE=2
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

define execute
$(1)

endef

tests/test_%: tests/test_%.c tests/test.c $(SRCS)
	$(CC) -Isrc -Iinclude -std=c89 -o $@ $^

.PHONY: test
test: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, ./$(t)))
	$(MAKE) -C libunicode test

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) tags
	$(MAKE) -C libunicode clean

.PHONY: veryclean
veryclean: clean
	$(RM) compile_commands.json
	$(MAKE) -C libunicode veryclean

.PHONY: lsp
lsp: compile_commands.json libunicode/compile_commands.json

compile_commands.json: $(SRCS)
	bear -- make MAKE=true -B

libunicode/compile_commands.json: force_look
	$(MAKE) -C libunicode lsp

-include $(DEPS)

force_look:
	true
