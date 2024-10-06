
# set the upstream server's DB.

export MONGO_PORT_27017_DB=upstream
export NODE=false

bundle exec cucumber --tag "@upstream" -r features/step_definitions -r features/support -r build/_deps/nodes-test-src/features 
