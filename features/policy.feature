Feature: Policy

   Background:
      And the server has id "4ecaf2b6-42f2-49fe-b0ef-123758a3a4db"
      And there are users:
         | name      | admin  | fullname  |
         | tracy     | true   | Tracy     |
         | alice     | false  | Alice     |
         
      And there are groups:
         | name         |
         | Group 1      |
      And there are policies:
         | name   | viewuser        | viewgroup | edituser              | editgroup | execuser | execgroup |
         | p1     |                 | Group 1   |                       | Group 1   |          | Group 1   |
         | p2     |                 | Group 1   | tracy                 | Group 1   |          | Group 1   |
      And groups have policies:
         | name            | policy |
         | Group 1         | p1     |
      And the security indexes are generated
   
      And there are users in group "Group 1":
         | name      | roles  |
         | tracy     |        |
         | alice     |        |

      And there are streams:
         | name      | policy |
         | BugBase   | p1     |
         
      And there are sites:
         | headerTitle     | streamBgColor   |
         | Local           | lightgreen      |
 
	@javascript
	Scenario: Alice can be added to a policy in streams
      When "tracy" log into app "admin"
	   And she clicks "Streams"
      And she clicks button named "edit" in row with text "BugBase"
      And eventually the page contains "Group 1"
      And she clicks "ADD USER"
      And eventually a modal dialog appears
      And she enters "alice" in "query"
      And eventually 1 row appears
      And she adds all security
      And she clicks "SAVE POLICY"
      And eventually the page contains "saved"
      And eventually the page contains "Group 1"
      And eventually the page contains "Alice"
   