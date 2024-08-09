Feature: Nodes

   Background:
      And the server has id "6eae665a-f376-472d-87e5-5159c2e0b389"
      When there is an info
      When there is default security

 	@javascript
	Scenario: A server can be reset
      When "tracy" logs into admin
      And she clicks "RESET"    
      And eventually a modal dialog appears
      And she clicks "Ok"    
      And eventually the modal dialog disappears
      And eventually the page does not contain "RESET"
      And she clicks "Nodes"    
      And eventually the page contains "Upstream node"
      