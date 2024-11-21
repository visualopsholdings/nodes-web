Feature: Teams

   A user can create and view teams.

   Background:
      And there is default security
      And the server has infos:
         | type         | text                                     |
         | privateKey   | t5R[&?W.8-mjIc]cs#<T(5AW7sCJYHo^<2k-]!2! |
         | pubKey       | 31r]#CW@p}=]xYRwvg+P>NrDffBG}wXL0%t:[j6( |

      And there are groups:
         | name         |
         | Team 2      |
         | Team 3      |
      And groups have policies:
         | name         | policy |
         | Team 2       | p1     |
         | Team 3       | p1     |

      And there are users in group "Team 2":
         | name      |
         | leanne    |
 
      And the DB is new
      
      And "tracy" logs into admin
      And eventually the page contains "Welcome Tracy"
	   And she clicks "Teams"
      And eventually table has 4 rows

	@javascript
	Scenario: A new team can be created
	   When she clicks "ADD"
      And a modal dialog appears
      And she enters "Team 4" in "name"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually there is a group "Team 4" in the DB
      And eventually table has 5 rows
	   And she clicks "ADD"
      And a modal dialog appears
      And she enters "Team 5" in "name"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually there is a group "Team 5" in the DB
      And eventually table has 6 rows

	@javascript
	Scenario: A team name can be edited
      And she clicks button named "edit" in row with text "Team 1"
      And she enters "x" in "name"
	   And she clicks "SAVE"
      Then eventually there is a group "x" in the DB

   @javascript
	Scenario: A team can be deleted
      And she clicks button named "delete" in row with text "Team 3"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually table has 3 rows
      And eventually there are 4 groups in the DB
	   And she clicks "PURGE 1 TEAM"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually there are 3 groups in the DB
      
	@javascript
	Scenario: A user can be added to a team
      And she clicks button named "edit" in row with text "Team 2"
	   And she clicks "ADD MEMBER"
      And eventually a modal dialog appears
      And she enters "tracy" in "query"
      And she clicks button named "add" in row with text "Tracy"
      And eventually the modal dialog disappears
      And eventually table has 2 rows

	@javascript
	Scenario: A user can be deleted from a team
      And she clicks button named "edit" in row with text "Team 1"
      And she clicks button named "delete" in row with text "Tracy"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually table has 2 rows

