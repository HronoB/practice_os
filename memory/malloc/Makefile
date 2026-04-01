CC = gcc
CFLAGS = -g -O3 -m64 -no-pie -fno-builtin -Wall -fno-stack-protector
TARGET = main
SOURCES = main.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean



