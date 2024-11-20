#!/bin/bash
# 
# Run the server standalone after building.

build/nodes-web --logLevel=trace --httpPort=8081 --test
