DOCKER_IMG = x86-asm-toolchain:latest
PLATFORM = --platform linux/amd64
VOL = -v $$PWD:/ws -w /ws

RESULT = main
#CFLAGS = -g -fPIC -Wall -Werror
CFLAGS = -g -fsanitize=address -fPIC -Wall -Werror

compile:
	docker run --rm $(PLATFORM) $(VOL) $(DOCKER_IMG) \
		gcc main.c fiber.c fiber_asm.S -o $(RESULT) $(CFLAGS)

run: compile
	docker run --rm $(PLATFORM) $(VOL) $(DOCKER_IMG) ./$(RESULT)

clean:
	rm -f $(RESULT)

.PHONY: compile run clean