Feature: Nodes

   Background:
      And the server has id "4ecaf2b6-42f2-49fe-b0ef-123758a3a4db"
      When there is default security

      And there are sites:
         | headerTitle     | streamBgColor   |
         | Local           | lightgreen      |

      And the server has nodes:
         | id                                   | headerTitle  | build  |
         | 2d105350-8598-4377-b045-238194e23bc5 | Server 1     | 1      |
         | 14ee64a1-d68f-4f6f-862a-d1ea23210010 | Server 2     | 2      |
         | f2ecaf81-943d-4ffd-a3d4-fc2d2d48c1c6 | Server 3     | 3      |
                  
      And the DB is new      
      
 	@javascript
	Scenario: Nodes are displayed
      When "tracy" logs into admin
      And she clicks "Nodes"
      And eventually table has 3 rows
 