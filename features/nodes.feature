Feature: Nodes

   Background:
      And the server has id "4ecaf2b6-42f2-49fe-b0ef-123758a3a4db"
      When there is default security

      And there are sites:
         | headerTitle     | streamBgColor   |
         | Local           | lightgreen      |

      And the server has nodes:
         | id                                   | headerTitle  | pubKey | build  |
         | 2d105350-8598-4377-b045-238194e23bc5 | Server 1     | aaa    | 1      |
         | 14ee64a1-d68f-4f6f-862a-d1ea23210010 | Server 2     | bbb    | 2      |
         | f2ecaf81-943d-4ffd-a3d4-fc2d2d48c1c6 | Server 3     | ccc    | 3      |
                  
      And the DB is new      
      
      When "tracy" logs into admin
      And she clicks "Nodes"
      And eventually table has 3 rows

 	@javascript
	Scenario: A node can be edited
      When she clicks button named "edit" in row with text "Server 2"
      Then eventually the page contains "bbb"

 	@javascript
	Scenario: A node can be added
	   When she clicks "ADD"
      And a modal dialog appears
      And she enters "aaaa" in "serverId"
      And she enters "bbbb" in "pubKey"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually table has 4 rows

	@javascript
	Scenario: A node can be deleted
      When she clicks button named "delete" in row with text "Server 2"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually table has 2 rows
