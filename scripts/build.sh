#!/bin/bash
# 
# Build the project for http access

if [ ! -d "nodes-web" ];
then
  echo "nodes-web project missing."
  exit 1
fi

if [ ! -d "nodes-web/build" ];
then
  mkdir nodes-web/build
fi
pushd nodes-web/build
git pull
cmake ..
make -j4
popd

pushd nodes-web/angular/login-ui
npm install
./build.sh
popd

pushd nodes-web/angular/admin-ui
npm install
./build.sh
popd

pushd nodes-web/angular/chat-ui
npm install
./build.sh
popd
