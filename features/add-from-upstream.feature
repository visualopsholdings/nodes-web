@needsupstream
Feature: Add From Upstream

   Background:
      When there is default security
      
      And the server has id "28d243ab-505e-4d81-a3e9-c314a8ef9761"
      And the server has privateKey "0F(P!Ik}GR9uP3JzezQ=+@$++qXAfP<-1^<Q4dWJ"
      And the server has pubKey "PUYdHOr65gaR*oW6^XxaCIR3-VN=x4W{nDlAfP-7"
      And the server has upstream "localhost"
      And the server has upstreamPubKey "31r]#CW@p}=]xYRwvg+P>NrDffBG}wXL0%t:[j6("
 
      And there are sites:
         | headerTitle     | streamBgColor   |
         | Test            | lightgreen      |

      And nodes is reloaded
      And she waits 2 seconds
      
  @javascript
  Scenario: A user can be added from upstream
      When "tracy" logs into admin
      And she clicks "Users"
      And eventually table has 5 rows
      
      # add Paul
      And she clicks "ADD FROM UPSTREAM"
      And a modal dialog appears
      And she enters "Joe" in "email"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually a modal dialog appears
      And eventually the page contains "Joe Bloggs"
      And she checks the first list option
      And she clicks "Ok"
      And eventually the modal dialog disappears
      Then eventually the page contains "Discovery complete"
      And eventually table has 6 rows
      And eventually the user fullname "Joe Bloggs" with salt and hash appears in the DB

      # add Marvin
      And she clicks "ADD FROM UPSTREAM"
      And a modal dialog appears
      And she enters "Fred" in "email"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually a modal dialog appears
      And eventually the page contains "Fred Bloggs"
      And she checks the first list option
      And she clicks "Ok"
      And eventually the modal dialog disappears
      Then eventually the page contains "Discovery complete"
      And eventually table has 7 rows
      And eventually the user fullname "Fred Bloggs" with salt and hash appears in the DB

  @javascript
  Scenario: A team can be added from upstream
      When "tracy" logs into admin
      And she clicks "Teams"
      And eventually table has 2 rows
      
      # add Paul
      And she clicks "ADD FROM UPSTREAM"
      And a modal dialog appears
      And she enters "New" in "name"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually a modal dialog appears
      And eventually the page contains "New Team"
      And she checks the first list option
      And she clicks "Ok"
      And eventually the modal dialog disappears
      Then eventually the page contains "Discovery complete"
      And eventually table has 3 rows
      And eventually there is a group "New Team" in the DB
