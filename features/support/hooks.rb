require "service_manager"

Before do |scenario|
   ServiceManager.stop
   ServiceManager.start
end

require 'database_cleaner-mongoid'
DatabaseCleaner.strategy = :deletion

Before do |scenario|
   DatabaseCleaner.clean
end
