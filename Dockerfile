FROM alpine:3.19

ENV VERSION=13.2.rel1
ENV PACKAGE=arm-gnu-toolchain-$VERSION-x86_64-arm-none-eabi

RUN mkdir build && \
    mkdir -p /opt/gcc-arm-none-eabi && \
    apk update && \
    apk add --no-cache \
    # Package dependencies for the toolchain
    gcompat=1.1.0-r4 \
    libstdc++=13.2.1_git20231014-r0 \
    libgcc=13.2.1_git20231014-r0 \
    openocd=0.12.0-r4 && \
    # Install tar, wget and xz to download and extract the toolchain
    apk add --no-cache tar=1.35-r2 xz=5.4.5-r0 wget=1.21.4-r0 && \
    # Download and extract the toolchain
    wget --progress=dot:giga -O archive.tar.xz "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/$PACKAGE.tar.xz" && \
    tar xf archive.tar.xz --strip-components=1 -C /opt/gcc-arm-none-eabi && \
    rm archive.tar.xz && \
    # Remove  uncessary packages
    apk del tar xz wget

ENV PATH "/opt/gcc-arm-none-eabi/bin:$PATH"

RUN echo 'Validate gcc location' && which arm-none-eabi-gcc && \
    echo 'Validate gcc version' && arm-none-eabi-gcc --version

WORKDIR /build