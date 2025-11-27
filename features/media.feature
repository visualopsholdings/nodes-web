Feature: Media

   Background:
      And the server has id "6eae665a-f376-472d-87e5-5159c2e0b389"
      When there is an info
      When there is default security

      And there are media:
         | name    | policy |
         | Item 1  | p1     |
         | Item 2  | p1     |
         | Item 3  | p1     |
         | Item 4  | p3     |
         
      And the DB is new      
      
      When "tracy" log into app "admin"
      And she clicks "Media"
      And eventually table has 4 rows

	@javascript
	Scenario: A media can be deleted
      When she clicks button named "delete" in row with text "Item 4"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually table has 3 rows
      And eventually there are 4 media in the DB
	   And she clicks "PURGE 1 MEDIA"
      And a modal dialog appears
      And she clicks "Ok"
      And eventually there are 3 media in the DB
