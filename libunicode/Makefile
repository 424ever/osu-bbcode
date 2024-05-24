CFLAGS += -std=c89
CFLAGS += -Wall -Wextra -Werror --pedantic-errors
CFLAGS += -MMD
CFLAGS += -Isrc
CFLAGS += -Iinclude

SRCS    = src/unicode.c
OBJS    = $(SRCS:.c=.o)
DEPS    = $(SRCS:.c=.d)
LIB     = libunicode.a
TESTS   = $(patsubst tests/%.c, tests/%, $(wildcard tests/test_*.c))

$(LIB): $(OBJS)
	$(AR) rcs $@ $^ $(LDFLAGS)

define execute
$(1)

endef

tests/test_%: tests/test_%.c tests/test.c $(SRCS)
	$(CC) -Isrc -Iinclude -Itests -std=c89 -Wno-unused-function -o $@ $^

.PHONY: test
test: $(TESTS)
	$(foreach t, $(TESTS), $(call execute, ./$(t)))

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) $(LIB)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(wildcard tests/*[^ch])
	$(RM) tags
	$(RM) compile_commands.json

.PHONY: lsp
lsp: compile_commands.json

compile_commands.json: $(SRCS)
	bear -- make -B


-include $(DEPS)
