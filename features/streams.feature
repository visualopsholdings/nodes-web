Feature: Streams

   Background:
      And the server has id "6eae665a-f376-472d-87e5-5159c2e0b389"
      When there is an info
      When there is default security

      And there are streams:
         | name            | policy |
         | Conversation 1  | p1     |
         | Conversation 2  | p1     |
         | Conversation 3  | p1     |
         | Conversation 4  | p3     |
         
     When "tracy" log into app "admin"
     And she clicks "Streams"
   And eventually table has 4 rows

	@javascript
	Scenario: A stream name can be edited
      When she clicks button named "edit" in row with text "Conversation 1"
      And she enters "x" in "name"
	   And she clicks "SAVE"
      Then eventually there is a stream "x" in the DB

	@javascript
	Scenario: A new stream can be created in admin
	   When she clicks "ADD"
      And a modal dialog appears
      And she enters "My new stream" in "name"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually table has 5 rows
      And she clicks button named "edit" in row with text "My new stream"
      And eventually the page contains "SAVE"

	@javascript
	Scenario: A stream can be deleted
      When she clicks button named "delete" in row with text "Conversation 4"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually table has 3 rows
      And eventually there are 3 streams in the DB
