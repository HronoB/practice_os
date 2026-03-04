# Указываем платформу x86_64
FROM --platform=linux/amd64 ubuntu:22.04

RUN apt-get update && apt-get install -y \
    gcc \
    make \
    gdb \
    strace \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspac

VOLUME /app

# Копируем исходники
COPY . /workspace/

# Собираем проект (теперь точно для x86_64)
RUN make CFLAGS="-g -nostdlib -static -no-pie -fno-stack-protector -m64"

CMD ["./syscall_program"]