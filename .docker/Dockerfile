FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Chicago
ENV TERM=xterm

WORKDIR /usr/src/libtrossen_arm

RUN \
  apt-get update -yqq && \
  apt-get upgrade -yqq && \
  apt-get install -yqq --no-install-recommends \
    build-essential=12.9ubuntu3 \
    cmake=3.22.1-1ubuntu1.22.04.2 && \
  rm -rf /var/lib/apt/lists/*

COPY ../ .

WORKDIR /usr/src/libtrossen_arm

RUN \
  make install-demos && \
  rm -rf /usr/src/libtrossen_arm

CMD [ "/usr/local/bin/configure_cleanup" ]
