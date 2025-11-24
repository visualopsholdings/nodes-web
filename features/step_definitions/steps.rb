require 'json'
require 'capybara'

saved_clip_text = ""

When('{string} logs into chat') do |name|
  visit("http://localhost:8081/login/?username=" + name + "&app=chat")
end

When('{string} logs into admin') do |name|
  visit("http://localhost:8081/login/?username=" + name + "&app=admin")
end

When('{string} logs into react chat') do |name|
  visit("http://localhost:8081/login/?username=" + name + "&app=rchat")
end

Given('{string} log into app {string}') do |name, app|
  visit("http://localhost:8081/login/?username=" + name + "&app=" + app)
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

Then(/^eventually table has (\d+) rows$/) do |count|
   eventually { expect(find('table > tbody')).to have_selector('tr', count: count.to_i) }
end

Given(/^she enters "(.*?)" in "(.*?)"$/) do |text, field|
   element = find_field(field)
   element.native.clear
   fill_in(field, :with => text)
   eventually { expect(find_field(field).value).to eq text }
   find_field(field).native.send_keys :tab
end

When('she refreshes the page') do
   page.driver.browser.navigate.refresh
end

When("she waits {int} seconds") do |n|
  sleep(n.to_i)
end

When('nodes is reloaded') do
   j = JSON.generate({ "type": "reload" })
   result = JSON.parse(`$NODES_HOME/build/Send --logLevel=trace '#{j}'`)
   expect(result["type"]).to eq("ack")
end

Given("she clicks button named {string} in row with text {string}") do |name, text|
   find(:xpath, "//table/tbody/tr/td[text()='#{text}']/..").find("button[name='#{name}']").click()
end

When("eventually {int} row appears") do |count|
   eventually { expect(find('tbody')).to have_selector('tr', count: count.to_i) }
end

When(/^she clicks button named "([^"]*)"$/) do |name|
   find("button[name='#{name}']").click()
end

When("the clipboard contains {string}") do |str|
   clip_text = page.evaluate_async_script("navigator.clipboard.readText().then(arguments[0])")
   expect(clip_text).to have_content(str)
end

When('the clipboard contents is saved') do
   saved_clip_text = page.evaluate_async_script("navigator.clipboard.readText().then(arguments[0])")
   puts saved_clip_text
end

When('she navigates to login url that was on clipboard') do
   visit("http://localhost:8081/logout")
   visit(saved_clip_text)
end

When('the DB is new') do
   j = JSON.generate({ "type": "reload" })
   result = JSON.parse(`$NODES_HOME/build/Send --logLevel=trace '#{j}'`)
   expect(result["type"]).to eq("ack")
end

When('she uploads file {string}') do |filename|
  page.attach_file("fileUpload", filename, make_visible: true)
end
