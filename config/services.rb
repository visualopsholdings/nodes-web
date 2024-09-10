require 'service_manager'

# ServiceManager.define_service 'vops' do |s|
#    s.start_cmd = 'cd $VOPS_HOME; MONGO_PORT_27017_TCP_ADDR=127.0.0.1 MONGO_PORT_27017_TCP_PORT=27017 node test-app.js -p 3000 --memcached'
#    s.loaded_cue = /start_tests/
#    s.cwd = Dir.pwd
#    s.pid_file = 'vops.pid'
# end

ServiceManager.define_service 'nodes' do |s|
   s.start_cmd = '$NODES_HOME/build/nodes --test --logLevel=debug --noupstream'
   s.loaded_cue = /Local REP/
   s.cwd = Dir.pwd
   s.pid_file = 'nodes.pid'
end

ServiceManager.define_service 'nodes-web' do |s|
   s.start_cmd = 'build/nodes-web --logLevel=debug --httpPort=8081 --test'
   s.loaded_cue = /Local REQ/
   s.cwd = Dir.pwd
   s.pid_file = 'nodes-web.pid'
end
