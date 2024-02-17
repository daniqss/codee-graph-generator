#!/bin/bash
cd ./master
docker build -t buildbot-master-image:master .

cd ..
cd ./worker
docker build -t buildbot-worker-image:worker .

cd ..
docker compose up