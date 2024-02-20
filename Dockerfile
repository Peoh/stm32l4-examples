FROM debian:bookworm-slim

RUN mkdir build
RUN mkdir -p /opt/gcc-arm-none-eabi

ENV VERSION=13.2.rel1
ENV PACKAGE=arm-gnu-toolchain-$VERSION-x86_64-arm-none-eabi

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y make cmake zip curl wget git doxygen graphviz python3 python3-pip

RUN wget -O archive.tar.xz "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/$PACKAGE.tar.xz" && \
    tar xf archive.tar.xz --strip-components=1 -C /opt/gcc-arm-none-eabi && \
    rm archive.tar.xz

RUN apt-get install -y openocd

RUN apt-get autoclean -y && \
    apt-get autoremove -y && \
    apt-get clean



ENV PATH "/opt/gcc-arm-none-eabi/bin:$PATH"

RUN echo 'Validate gcc location' && which arm-none-eabi-gcc && \
    echo 'Validate gcc version' && arm-none-eabi-gcc --version

WORKDIR /build