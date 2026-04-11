CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11 -Iinclude

SRCS    = src/main.c   \
          src/color.c  \
          src/args.c   \
          src/git.c    \
          src/output.c

OBJS    = $(SRCS:.c=.o)

# ── detect OS ──────────────────────────────────
ifeq ($(OS), Windows_NT)
    TARGET  = git-recall.exe
    INSTALL_DIR = C:/Windows/System32
    RM      = del /Q
    INSTALL_CMD = copy $(TARGET) $(INSTALL_DIR)\$(TARGET)
    UNINSTALL_CMD = del /Q $(INSTALL_DIR)\$(TARGET)
    INSTALL_MSG = "Installed! Try: git-recall --week"
    UNINSTALL_MSG = "Uninstalled git-recall"
else
    TARGET  = git-recall
    INSTALL_DIR = /usr/local/bin
    RM      = rm -f
    INSTALL_CMD = cp $(TARGET) $(INSTALL_DIR)/$(TARGET) && chmod 755 $(INSTALL_DIR)/$(TARGET)
    UNINSTALL_CMD = rm -f $(INSTALL_DIR)/$(TARGET)
    INSTALL_MSG = "✅  Installed!  Try: git recall"
    UNINSTALL_MSG = "🗑️   Uninstalled git-recall"
endif

# ── build ──────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# ── install / uninstall ────────────────────────
install: $(TARGET)
	$(INSTALL_CMD)
	@echo $(INSTALL_MSG)

uninstall:
	$(UNINSTALL_CMD)
	@echo $(UNINSTALL_MSG)

# ── clean ──────────────────────────────────────
clean:
	$(RM) $(OBJS) $(TARGET)

.PHONY: all install uninstall clean