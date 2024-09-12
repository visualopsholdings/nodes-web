Feature: Manual

   @javascript
	Scenario: setup for manual testing
      And the server has id "6eae665a-f376-472d-87e5-5159c2e0b389"
      When there is an info
      When there is default security

      And there are streams:
         | name               | policy | streambits   |
         | My Conversation 1  | p1     | 2048         |
         | My Conversation 2  | p1     | 0            |

      And there are ideas:
         | name         | stream             | policy | by     |
         | Message 1    | My Conversation 1  | p1     | tracy  |
         | Message 2    | My Conversation 1  | p1     | leanne |
         | Message 3    | My Conversation 1  | p1     | tracy  |
         | Message 4    | My Conversation 1  | p1     | tracy  |
         | Message 5    | My Conversation 1  | p1     | leanne |
         | Message 6    | My Conversation 2  | p1     | tracy  |
         | Message 7    | My Conversation 2  | p1     | tracy  |
         | Message 8    | My Conversation 2  | p1     | tracy  |
         | Message 9    | My Conversation 2  | p1     | leanne |
 
       And there are sites:
         | headerTitle     | streamBgColor   |
         | Local           | lightgreen      |
