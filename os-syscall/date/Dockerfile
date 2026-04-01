FROM --platform=linux/amd64 ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install base tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    gcc-multilib \
    g++-multilib \
    gdb \
    qemu-system-x86 \
    qemu-utils \
    git \
    make \
    perl \
    python3 \
    curl \
    wget \
    vim \
    bison \
    flex \
    libgmp3-dev \
    && rm -rf /var/lib/apt/lists/*

# Install proper i386-elf cross-compiler from source (Ubuntu packages don't support asm constraints)
RUN apt-get update && apt-get install -y \
    build-essential \
    texinfo \
    gawk \
    && rm -rf /var/lib/apt/lists/*

# Build i386-elf-gcc cross-compiler
WORKDIR /tmp
RUN git clone --depth=1 --branch gcc-13.2.0 https://github.com/gcc-mirror/gcc.git gcc-src && \
    git clone --depth=1 --branch binutils-2.41 https://sourceware.org/git/binutils-gdb.git binutils-src && \
    mkdir gcc-build binutils-build && \
    cd binutils-build && \
    ../binutils-src/configure --target=i386-elf --prefix=/usr/local/i386-elf --disable-nls && \
    make -j$(nproc) && make install && \
    cd ../gcc-build && \
    ../gcc-src/configure --target=i386-elf --prefix=/usr/local/i386-elf \
        --disable-nls --enable-languages=c,c++ --without-headers && \
    make all-gcc -j$(nproc) && make all-target-libgcc -j$(nproc) && make install-gcc && \
    make install-target-libgcc && \
    cd / && rm -rf /tmp/gcc*

# Verify toolchain
RUN /usr/local/i386-elf/bin/i386-elf-gcc --version

# Set environment variables
ENV TOOLPREFIX=i386-elf-
ENV PATH="/usr/local/i386-elf/bin:${PATH}"

# Create user
ARG USERNAME=developer
ARG USER_UID=1000
ARG USER_GID=1000
RUN groupadd --gid $USER_GID $USERNAME && \
    useradd --uid $USER_UID --gid $USER_GID -m $USERNAME

USER $USERNAME
WORKDIR /home/$USERNAME/project
CMD ["/bin/bash"]