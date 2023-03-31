# Basic dockerized environment for building and using TinyTP
# Taken from: https://github.com/JetBrains/clion-remote/blob/master/Dockerfile.cpp-env-ubuntu
FROM ubuntu:20.04

RUN DEBIAN_FRONTEND="noninteractive" apt-get update && apt-get -y install tzdata

RUN apt-get update \
  && apt-get install -y build-essential \
      gcc \
      g++ \
      gdb \
      clang \
      make \
      ninja-build \
      cmake \
      autoconf \
      automake \
      libtool \
      valgrind \
      locales-all \
      dos2unix \
      rsync \
      tar \
      python3 \
      python3-dev \
      curl \
      git \
  && apt-get clean