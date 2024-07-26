require 'service_manager'

ServiceManager.define_service 'zmqchat' do |s|
   s.start_cmd = '$ZMQCHAT_HOME/build/zmqchat --test --logLevel=trace'
   s.loaded_cue = /Local REP/
   s.cwd = Dir.pwd
   s.pid_file = 'zmqchat.pid'
end

ServiceManager.define_service 'zchttp' do |s|
   s.start_cmd = 'build/zchttp --logLevel=trace'
   s.loaded_cue = /init accept #0/
   s.cwd = Dir.pwd
   s.pid_file = 'zchttp.pid'
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
