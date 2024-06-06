CFLAGS += -std=c89
CFLAGS += -Wall -Wextra -Werror --pedantic-errors
CFLAGS += -Iinclude

SRCS    = src/test.c
OBJS    = $(SRCS:.c=.o)
DEPS    = $(SRCS:.c=.d)
LIB     = libtest.a

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
	$(RM) compile_commands.json

.PHONY: lsp
lsp: compile_commands.json

compile_commands.json: $(SRCS)
	bear -- make -B

%.d: %.c
	$(CC) -MM $(CFLAGS) $^

-include $(DEPS)
