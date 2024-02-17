FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

# Instalar dependencias
RUN apt update && \
    apt upgrade -y && \
    apt install -y build-essential \
                   python3 \
                   nano \
                   python3-dev \
                   python3-pip \
                   python-is-python3 \
                   libssl-dev \
                   libffi-dev \
                   git \
                   nodejs

# Actualizar pip e instalar Buildbot y psycopg2-binary
RUN python3 -m pip install --upgrade pip && \
    python3 -m pip install 'buildbot[bundle]==3.11.0' psycopg2-binary==2.9.9

# Configurar el directorio de trabajo
WORKDIR /buildbot
RUN buildbot create-master master

# Crear el directorio 'results' y copiar los archivos desde el directorio local al contenedor
RUN mkdir -p /buildbot/results
COPY master.cfg /buildbot/master/master.cfg
COPY server.js /buildbot/server.js

# Crear el directorio 'master' y copiar el Dockerfile al contenedor
RUN mkdir -p /buildbot/master

# Exponer los puertos
EXPOSE 8010
EXPOSE 80

# Comando de entrada
# RUN node /buildbot/server.js
ENTRYPOINT buildbot upgrade-master master && buildbot start --nodaemon master