require 'service_manager'

ServiceManager.define_service 'nodes' do |s|
   s.start_cmd = '$NODES_HOME/build/nodes --test --logLevel=trace'
   s.loaded_cue = /Local REP/
   s.cwd = Dir.pwd
   s.pid_file = 'nodes.pid'
end

ServiceManager.define_service 'nodes-web' do |s|
   s.start_cmd = 'build/nodes-web --logLevel=trace'
   s.loaded_cue = /Local REQ/
   s.cwd = Dir.pwd
   s.pid_file = 'nodes-web.pid'
end

ServiceManager.define_service 'chat-app' do |s|
   s.start_cmd = 'cd angular/chat-ui; ng serve --proxy-config proxy.conf.js --port 8081'
   s.loaded_cue = /Angular Live Development Server is listening/
   s.cwd = Dir.pwd
   s.pid_file = 'chat-app.pid'
end

ServiceManager.define_service 'admin-app' do |s|
   s.start_cmd = 'cd angular/admin-ui; ng serve --proxy-config proxy.conf.js --port 8082'
   s.loaded_cue = /Angular Live Development Server is listening/
   s.cwd = Dir.pwd
   s.pid_file = 'admin-app.pid'
end

ServiceManager.define_service 'login-app' do |s|
   s.start_cmd = 'cd angular/login-ui; ng serve --proxy-config proxy.conf.js --port 8083'
   s.loaded_cue = /Angular Live Development Server is listening/
   s.cwd = Dir.pwd
   s.pid_file = 'login-app.pid'
end
