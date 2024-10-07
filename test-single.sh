#!/bin/bash
# 
# Run the cucumber tests

export MONGO_PORT_27017_DB=dev

bundle exec cucumber  -n "$1" --tag "not (@interactive)" -r features/step_definitions -r features/support -r build/_deps/nodes-test-src/features 
