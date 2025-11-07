#!/bin/bash
# 
# Run the nodes server

$NODES_HOME/build/nodes --test --logLevel=debug --schema=scripts/schema.json --hostName=localhost:8081
