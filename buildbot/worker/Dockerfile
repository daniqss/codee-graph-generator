FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt update
RUN apt upgrade -y
RUN apt install -y build-essential \
                   python3 \
                   python3-dev \
                   python3-pip \
                   python-is-python3 \
                   libssl-dev \
                   libffi-dev \
                   git \
                   cmake

RUN python -m pip install --upgrade pip
RUN python -m pip install buildbot-worker==3.11.0

WORKDIR /buildbot
RUN buildbot-worker create-worker worker buildbot-master-node worker worker

# required for `runtests` build
RUN python -m pip install setuptools-trial

ENTRYPOINT buildbot-worker start --nodaemon worker