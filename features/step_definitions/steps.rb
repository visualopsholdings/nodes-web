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

When(/^she clicks "(.*?)"$/) do |name|
  click_link_or_button(name)
end

When(/^a modal dialog appears$/) do
   find(".modal-dialog")
end

When(/^eventually a modal dialog appears$/) do
   eventually { find(".modal-dialog") }
end

When(/^eventually the modal dialog disappears$/) do
  eventually { has_no_css?(".modal-dialog") }
end

Then(/^eventually the page contains "([^"]*)"$/) do |text|
   eventually { expect(page).to have_content(text) }
end

Then(/^eventually the page does not contain "([^"]*)"$/) do |text|
   eventually { expect(page).not_to have_content(text) }
end

Given("before the end of time the page contains {string}") do |text|
   beforeTheEndOfTime { expect(page).to have_content(text) }
end
