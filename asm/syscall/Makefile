CC = gcc
CFLAGS = -g -nostdlib -static -no-pie -fno-stack-protector
TARGET = syscall_program
SOURCES = entry.S syscall.c main.c

# Проверяем архитектуру
ARCH = $(shell uname -m)
ifeq ($(ARCH),x86_64)
    CFLAGS += -m64
    $(info Building for x86_64)
else ifeq ($(ARCH),aarch64)
    CFLAGS += -march=armv8-a
    $(info Building for ARM64)
endif

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -I. -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) trace.log

trace: $(TARGET)
	strace -f -o trace.log ./$(TARGET)

.PHONY: all run clean trace