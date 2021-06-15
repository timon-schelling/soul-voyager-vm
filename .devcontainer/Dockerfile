FROM mcr.microsoft.com/vscode/devcontainers/cpp:focal
SHELL ["/bin/bash", "-c"]
RUN apt-get update && apt-get install -y \
    curl \
    wget \
    git \
    zip \
    unzip \
    rsync \
    build-essential \
    mingw-w64 \
    gcc \
    gdb \
    gdbserver \
    make \
    cmake \
    default-jdk

RUN curl -s "https://get.sdkman.io" | bash

RUN echo "sdkman_auto_answer=true" > /root/.sdkman/etc/config && \
    echo "sdkman_auto_selfupdate=false" >> /root/.sdkman/etc/config && \
    echo "sdkman_curl_connect_timeout=20" >> /root/.sdkman/etc/config && \
    echo "sdkman_curl_max_time=0" >> /root/.sdkman/etc/config

RUN source "/root/.sdkman/bin/sdkman-init.sh" && \
    sdk install kotlin && \
    sdk install kscript
