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

$(BIN): $(OBJS) libunicode/libunicode.a
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

libunicode/libunicode.a: force_look
	$(MAKE) -C libunicode

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) tags
	$(RM) compile_commands.json
	$(MAKE) -C libunicode clean

.PHONY: lsp
lsp: compile_commands.json libunicode/compile_commands.json

compile_commands.json: $(SRCS)
	bear -- make -B

libunicode/compile_commands.json: force_look
	$(MAKE) -C libunicode lsp

-include $(DEPS)

force_look:
	true
