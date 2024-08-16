Feature: Users

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

 	@javascript
	Scenario: A user can be added from upstream
      When "tracy" logs into admin
	   And she clicks "Users"
	   And she clicks "ADD FROM UPSTREAM"
      And eventually a modal dialog appears
      And she clicks "Ok"    
      And eventually the modal dialog disappears
     