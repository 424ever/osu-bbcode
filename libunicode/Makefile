CFLAGS  = -std=c89
CFLAGS += -Wall -Wextra -Werror --pedantic-errors
CFLAGS += -MMD
CFLAGS += -Isrc
CFLAGS += -Iinclude

SRCS    = src/unicode.c
OBJS    = $(SRCS:.c=.o)
DEPS    = $(SRCS:.c=.d)
LIB     = libunicode.a

$(LIB): $(OBJS)
	$(AR) rcs $@ $^ $(LDFLAGS)

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(LIB)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) tags
	$(RM) compile_commands.json

-include $(DEPS)

tags: $(SRCS)
	ctags -f $@ -R src $^
