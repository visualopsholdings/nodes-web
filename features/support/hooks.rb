require "service_manager"
require 'database_cleaner-mongoid'

DatabaseCleaner.strategy = :deletion

Before do |scenario|
   DatabaseCleaner.clean
end

Before('@restart') do
   ServiceManager.stop
   ServiceManager.start
end

