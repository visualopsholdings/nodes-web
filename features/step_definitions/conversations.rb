

Then("eventually there are {int} items") do |count|
   eventually { expect(find('.listcontent')).to have_selector('.item', count: count.to_i) }
end

Then("the last item has text {string}") do |text|
   expect(find(".listcontent div.items:last-child .text").text()).to eq(text)
end

When("she clicks menu on first action in conversation item with title {string}") do |title|
   find(".listcontent .item .text", :text => title).find(:xpath, "../../../..").find(".menu button").click()
end
