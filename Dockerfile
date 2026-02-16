FROM ubuntu:24.04

ENV TZ Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update --fix-missing
RUN apt-get install -y software-properties-common

RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test
RUN apt-get update --fix-missing
RUN apt-get install -y \
    build-essential \
    gcc-multilib \
    gcc-14 \
    g++-14
RUN update-alternatives \
    --install /usr/bin/gcc gcc /usr/bin/gcc-14 40 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-14

RUN apt-get update --fix-missing
RUN apt-get install -y qemu-system-x86

WORKDIR /workspace
