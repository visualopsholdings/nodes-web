
require "service_manager"

# don't run NODE sometimes.
puts "node:", ENV['NODE']
if ENV['NODE'] != "false"
   ServiceManager.start
end
