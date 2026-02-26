all: run

RESULT=main.exe

compile: main.c Makefile context_impl.S
	gcc -g -m64 -mcmodel=medium -Wl,--image-base=0x400000 context_impl.S main.c -I. -o $(RESULT)

run: compile
	$(RESULT)

clean:
	del /f $(RESULT) *.o