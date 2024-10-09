Feature: Chat (React)

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
      When "leanne" logs into react chat
      And eventually the page contains "Welcome Leanne"
