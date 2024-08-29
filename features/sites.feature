Feature: Sites

   Background:
      And the server has id "6eae665a-f376-472d-87e5-5159c2e0b389"
      When there is default security

      And there are sites:
         | headerTitle     | streamBgColor   |
         | Local           | lightgreen      |

 	@javascript
	Scenario: A site can be renamed
      When "tracy" logs into admin
      And she clicks "Site"
      And she enters "My Site" in "headerTitle"
      And she clicks "SAVE"
      And she clicks "Nodes"
      And she clicks "Site"
      And eventually there is a site "My Site" in the DB
