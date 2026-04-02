




DOCKER_IMG = x86-asm-toolchain:latest
PLATFORM = --platform linux/amd64
VOL = -v $$PWD:/ws -w /ws


compile:
	docker run --rm $(PLATFORM) $(VOL) $(DOCKER_IMG) \
		gcc -o main main.c -g -Wall -lpthread -static -O2


run: compile
	docker run --rm --platform linux/amd64 -v $(PWD):/ws -w /ws x86-asm-toolchain:latest ./main


main: compile


clean:
	rm -f main *.o
.PHONY: compile run clean main






































