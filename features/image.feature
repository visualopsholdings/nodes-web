Feature: Image

   The image sequence can be used.

   Background:
      And there is default security
      And the server has infos:
         | type         | text                                     |
         | privateKey   | t5R[&?W.8-mjIc]cs#<T(5AW7sCJYHo^<2k-]!2! |
         | pubKey       | 31r]#CW@p}=]xYRwvg+P>NrDffBG}wXL0%t:[j6( |
         
      And there are streams:
         | name            | policy    |
         | My Conversation 1  | p1     |

      And there are sites:
         | headerTitle     | streamBgColor   |
         | Local           | lightgreen      |

      And the DB is new
      And "tracy" logs into chat
      And eventually there are 1 streams
      
	@javascript
	Scenario: An image can be imported
      And she clicks "Conversation 1"
 	   And eventually there are 0 items
      And she enters "My Image" in "newText"
      And she clicks "add-image"
      And she uploads files "media/image.png" "media/linkedin.jpeg"
      And she clicks button named "upload"
      And eventually the page contains "Image upload successful."
 