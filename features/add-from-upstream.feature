Feature: Add From Upstream

   Background:
      When there is default security
      
      And the server has id "28d243ab-505e-4d81-a3e9-c314a8ef9761"
      And the server has privateKey "0F(P!Ik}GR9uP3JzezQ=+@$++qXAfP<-1^<Q4dWJ"
      And the server has pubKey "PUYdHOr65gaR*oW6^XxaCIR3-VN=x4W{nDlAfP-7"
      And the server has upstream "nodes.visualops.com"
      And the server has upstreamPubKey "K]<n72U1y#9PUS.j9BpC=XNxz6HCqhRfnGbSnajO"
 
      And there are sites:
         | headerTitle     | streamBgColor   |
         | Test            | lightgreen      |

      And nodes is reloaded
      And she waits 5 seconds
      
  @javascript
  Scenario: A user can be added from upstream
      When "tracy" logs into admin
      And she clicks "Users"
      And eventually table has 5 rows
      
      # add Paul
      And she clicks "ADD FROM UPSTREAM"
      And a modal dialog appears
      And she enters "Paul" in "email"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually a modal dialog appears
      And eventually the page contains "Paul Hamilton"
      And she checks the first list option
      And she clicks "Ok"
      And eventually the modal dialog disappears
      Then eventually the page contains "Discovery complete"
      And eventually table has 6 rows
      And eventually the user fullname "Paul Hamilton" with salt and hash appears in the DB

      # add Marvin
      And she clicks "ADD FROM UPSTREAM"
      And a modal dialog appears
      And she enters "Marvin" in "email"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      And eventually a modal dialog appears
      And eventually the page contains "Marvin Avilez"
      And she checks the first list option
      And she clicks "Ok"
      And eventually the modal dialog disappears
      Then eventually the page contains "Discovery complete"
      And eventually table has 7 rows
      And eventually the user fullname "Marvin Avilez" with salt and hash appears in the DB
