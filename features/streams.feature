Feature: Streams

   Background:
      And the server has id "6eae665a-f376-472d-87e5-5159c2e0b389"
      When there is an info
      When there is default security

      And there are streams:
         | name               | policy |
         | Conversation 1  | p1     |
         | Conversation 2  | p1     |

 	@javascript
	Scenario: Details for a stream can be displayed
      When "tracy" logs into chat
	   And eventually there are 2 streams
      