require 'json'
require 'capybara'

When('{string} logs into chat') do |name|
  visit("http://localhost:8081/login/?username=" + name)
end

When('{string} logs into admin') do |name|
  visit("http://localhost:8082/login/?username=" + name)
end

When('eventually there are {int} streams') do |count|
   eventually { expect(find('app-streams mat-nav-list')).to have_selector('a', count: count.to_i) }
end
