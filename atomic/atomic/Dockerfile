FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    make \
    gcc \
    g++ \
    nasm \
    binutils \
    gdb \
    libc6-dev \
    linux-libc-dev \
    libc-dev \
    pkg-config \
    git \
    curl \
    wget \
    vim \
    && rm -rf /var/lib/apt/lists/*

RUN cmake --version && \
    make --version && \
    gcc --version && \
    g++ --version && \
    nasm -v && \
    ld --version && \
    gdb --version

ENV PATH="/usr/bin:/usr/local/bin:${PATH}"
WORKDIR /workspace
CMD ["/bin/bash"]