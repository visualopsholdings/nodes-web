Feature: Share

   Background:
      And the server has id "6eae665a-f376-472d-87e5-5159c2e0b389"
      When there is default security

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
      And the clipboard contains "google"
