require 'mongoid'
require 'capybara/cucumber'
require 'selenium-webdriver'

def selenium_options
   options = Selenium::WebDriver::Chrome::Options.new(binary: "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome")
   options.add_preference('profile.content_settings.exceptions.clipboard', {
   '*': {'setting': 1}
   })
   options.add_argument("--verbose")
   options.add_argument("--disable-web-security")
   options
end

Capybara.register_driver :chrome do |app|

   Capybara::Selenium::Driver.new(app, :browser => :chrome, :options => selenium_options)
   
end
   
Capybara.javascript_driver = :chrome

Mongoid.load!("config/mongoid.yml", :test)
Mongoid.logger.level = Logger::INFO
Mongo::Logger.logger.level = Logger::INFO

Time.zone = 'UTC'
