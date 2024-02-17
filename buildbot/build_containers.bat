REM SPDX-FileCopyrightText: 2024 Gael Garcia Arias, Ivan Afonso Cerdeira, Daniel Queijo Seoane, Luca D'angel Sabin
REM
REM SPDX-License-Identifier: MIT

@echo off
cd .\master
docker build -t buildbot-master-image:master .

cd ..
cd .\worker
docker build -t buildbot-worker-image:worker .

cd ..
docker compose up
