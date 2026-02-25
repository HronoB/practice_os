all: run

RESULT=main.exe

compile: main.S Makefile sum.S
	gcc -g -m64 -mcmodel=medium -Wl,--image-base=0x400000 sum.S main.S -I. -o $(RESULT)

run: compile
	$(RESULT)

clean:
	del /f $(RESULT)