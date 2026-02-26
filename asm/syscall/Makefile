# Makefile для компиляции через WSL
# Конвертация Windows пути в WSL путь
WIN_PATH = $(subst \,/,$(subst \\wsl$\Ubuntu\,,$(CURDIR)))
ifeq ($(WIN_PATH),$(CURDIR))
    # Уже в WSL
else
    $(shell cd $(WIN_PATH))
endif




CC = gcc
CFLAGS = -g -m64 -nostdlib -static -no-pie
# Убираем -Wl,--image-base=0x400000 - это для Windows
LDFLAGS =
TARGET = syscall_program
SOURCES = entry.S syscall.c main.c

# Проверяем, что мы в WSL/Linux
ifeq ($(shell uname -s),Linux)
    $(info Building in WSL/Linux environment)
else
    $(error This makefile must be run in WSL. Use: wsl make)
endif

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -I. -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

# Для отладки с strace
trace: $(TARGET)
	strace -f -o trace.log ./$(TARGET)

.PHONY: all run clean trace