CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11 -Iinclude
TARGET  = git-recall

SRCS    = src/main.c   \
          src/color.c  \
          src/args.c   \
          src/git.c    \
          src/output.c

OBJS    = $(SRCS:.c=.o)

# ── build ──────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# ── install / uninstall ────────────────────────
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/$(TARGET)
	@echo "✅  Installed!  Try: git recall"

uninstall:
	rm -f /usr/local/bin/$(TARGET)
	@echo "🗑️   Uninstalled git-recall"

# ── clean ──────────────────────────────────────
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all install uninstall clean
