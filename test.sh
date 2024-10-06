#!/bin/bash
# 
# Run the cucumber tests

if [ "$1" == "features/add-from-upstream.feature" ];
then
  NEEDS_UPSTREAM=1
fi

if [ $NEEDS_UPSTREAM ]; 
then
  ci/upstreamdb.sh
  ci/run.sh upstream
  TAGS="not (@nodes or @broken or @interactive)"
else
  TAGS="not (@nodes or @broken or @interactive or @needsupstream)"
fi

export MONGO_PORT_27017_DB=dev

bundle exec cucumber $1 --tag "$TAGS" -r features/step_definitions -r features/support -r build/_deps/nodes-test-src/features 

if [ $NEEDS_UPSTREAM ]; 
then
  ci/kill.sh upstream
fi
