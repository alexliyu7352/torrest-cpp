# This image uses the context from scripts directory
# To build, run the following command from the project root directory:
# docker build -t i96751414/torrest-cpp-linux-x64:latest -f docker/linux-x64.Dockerfile scripts/

ARG CROSS_COMPILER_TAG=latest
FROM i96751414/cross-compiler-linux-x64:${CROSS_COMPILER_TAG}

ENV PREFIX "${CROSS_ROOT}"
ENV OATPP_OPTS -DOATPP_LINK_ATOMIC=OFF
ENV BOOST_CONFIG "using gcc : : ${CROSS_TRIPLE}-c++ ;"
ENV BOOST_OPTS target-os=linux
ENV OPENSSL_OPTS linux-x86_64
ENV OPENSSL_CROSS_COMPILE "${CROSS_TRIPLE}-"
# ENV CMAKE_TOOLCHAIN_FILE is already set on the base image

COPY install_dependencies.sh versions.env /tmp/
RUN /tmp/install_dependencies.sh --static \
    && rm /tmp/*
