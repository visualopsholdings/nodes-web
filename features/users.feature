Feature: Users

   Background:
      When there is default security
      And the server has infos:
         | type         | text                                     |
         | privateKey   | t5R[&?W.8-mjIc]cs#<T(5AW7sCJYHo^<2k-]!2! |
         | pubKey       | 31r]#CW@p}=]xYRwvg+P>NrDffBG}wXL0%t:[j6( |
            
      And the DB is new      
      When "tracy" logs into admin
 	   And she clicks "Users"
      And eventually table has 5 rows
      
	@javascript
	Scenario: A user can be set to admin
      And she clicks button named "edit" in row with text "Leanne"
      And she sets material checkbox 1
      And eventually a modal dialog appears
      And she clicks "Ok"
      And eventually the modal dialog disappears

   @javascript
	Scenario: A user can be deleted
      And she clicks button named "delete" in row with text "Leanne"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually table has 4 rows
      And eventually there are 5 users in the DB
	   And she clicks "PURGE 1 USERS"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually there are 4 users in the DB
      
