
When('she checks the first list option') do
   find("mat-selection-list > mat-list-option:nth-child(1)").click()
end

When(/^she sets material checkbox (\d+)$/) do |num|
   find("#mat-mdc-checkbox-#{num.to_i}").click()
end
