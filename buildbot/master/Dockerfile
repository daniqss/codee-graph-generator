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
                   git

RUN python -m pip install --upgrade pip
RUN python -m pip install 'buildbot[bundle]==3.11.0' psycopg2-binary==2.9.9

WORKDIR /buildbot
RUN buildbot create-master master
COPY master.cfg master/master.cfg

EXPOSE 8010
ENTRYPOINT buildbot upgrade-master master && buildbot start --nodaemon master
