Feature: Chat

   Background:
      And the server has id "6eae665a-f376-472d-87e5-5159c2e0b389"
      When there is default security

      And there are streams:
         | name               | policy |
         | My Conversation 1  | p1     |
         | My Conversation 2  | p1     |

      And there are ideas:
         | name         | stream             | policy | by     |
         | Message 1    | My Conversation 1  | p1     | tracy  |
         | Message 2    | My Conversation 1  | p1     | leanne |
 
      And there are sites:
         | headerTitle     | streamBgColor   |
         | Local           | lightgreen      |

      And the DB is new      
      
  @javascript
  Scenario: New messages can be created
      When "leanne" logs into chat
      And eventually there are 2 streams
      And she clicks "Conversation 1"
 	   And eventually there are 2 items
      And she enters "Hi there" in "newText"
      And she clicks "add-message"
 	   Then eventually there are 3 items
 	   And eventually there are 3 ideas in the DB

   @javascript
	Scenario: An idea can be deleted
      When "leanne" logs into chat
      And eventually there are 2 streams
	   And she clicks "Conversation 1"
 	   And eventually there are 2 items
	   And she clicks menu on first action in conversation item with title "Message 1"
	   And she clicks "DELETE"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually the modal dialog disappears
 	   And eventually there are 1 items
      And eventually there are 2 ideas in the DB

      And "tracy" log into app "admin"
	   And she clicks "Streams"
      And she clicks button named "edit" in row with text "My Conversation 1"
	   And she clicks "PURGE 1 IDEAS"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually there are 1 ideas in the DB
