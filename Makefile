CFLAGS  = -std=c89
CFLAGS += -Wall -Wextra -Werror --pedantic-errors
CFLAGS += -MMD
CFLAGS += -Isrc
CFLAGS += -D_POSIX_C_SOURCE=2

SRCS    = src/free.c \
	  src/main.c \
	  src/parse.c
OBJS    = $(SRCS:.c=.o)
DEPS    = $(SRCS:.c=.d)
BIN     = osu-bbcode

osu-bbcode: $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) tags

-include $(DEPS)

tags: $(SRCS)
	ctags -f $@ -R src $^
