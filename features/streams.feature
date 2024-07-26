Feature: Streams

   Background:
      When there is default security

      And there are streams:
         | name               | policy |
         | Conversation 1  | p1     |
         | Conversation 2  | p1     |

 	@javascript
	Scenario: Details for a stream can be displayed
      When "tracy" logs into chat
	   And eventually there are 2 streams
      