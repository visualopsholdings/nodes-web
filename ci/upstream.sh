
# run the server up as the upstream.

$NODES_HOME/build/nodes --test --logLevel=trace --dbName=upstream --schema=scripts/schema.json \
  --repPort=3113 --pubPort=3112 --dataReqPort=8820 --msgSubPort=8821
