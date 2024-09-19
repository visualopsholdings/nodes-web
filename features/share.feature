Feature: Share

   Background:
      And the server has id "6eae665a-f376-472d-87e5-5159c2e0b389"
      When there is default security
      And the server has infos:
         | type      | text                                     |
         | tokenKey  | 90B21444AC1A2C9146FFA34C72BF787F76E7B0EDD236F0508571264153E58A787B82729268EF67DFCCC6B1F113721B0D752DA65DA6BC82BCA9A1C73E58DAAFF7 |
         | tokenIV   | D764E7CAE16C361A4546873B |

      And there are streams:
         | name               | policy | streambits   |
         | My Conversation 1  | p1     | 2048         |
         | My Conversation 2  | p1     | 0            |

      And there are sites:
         | headerTitle     | streamBgColor   |
         | Local           | lightgreen      |

  @javascript
  Scenario: A stream can be shared with new users
      When "tracy" logs into chat
      And eventually there are 2 streams
      And she clicks "Conversation 1"
      And she clicks button named "share"
      And eventually a modal dialog appears
      And eventually the page contains "Team 1"
      And the clipboard contains "http://localhost:8081/apps/chat/#/streams/"
      And the clipboard contains "?token="
      And the clipboard contents is saved
      And she navigates to login url that was on clipboard
      And she enters "Bob" in "fullname"
      And she clicks "Login"
      And eventually the page contains "Please wait while we generate your Vops ID"
      And eventually a modal dialog appears
      And eventually the page contains "Save your VID"
      And she clicks "Continue"
      And eventually the modal dialog disappears
      And eventually the page contains "Welcome Bob"
      And eventually there are 2 streams
 	   When she clicks "Conversation 1"
 	   And eventually there are 0 items
      And she enters "Hi there" in "newText"
      And she clicks "add-message"
 	   Then eventually there are 1 items
 	   And eventually there are 1 ideas in the DB
