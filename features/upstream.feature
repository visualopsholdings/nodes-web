Feature: Upstream

   Background:
      When there is default security
      
      And the server has id "6868f9ac-46fe-48fd-9e00-33a1e1e256da"
      And the server has privateKey "su.uueDnW#]F{<4omtfxNNaZYZc{vAaok-qXB!s*"
      And the server has pubKey "DVv<okIiqc/N<)o:a{#x4re)hqJJH0)6?*J.T=5n"
 
      And there are sites:
         | headerTitle     | streamBgColor   |
         | Test            | lightgreen      |

      And nodes is reloaded
      
 	@javascript
	Scenario: The upstream server can be set
      When "tracy" logs into admin
      And she clicks "Nodes"
 	   And she clicks "SET UPSTREAM"
      And a modal dialog appears
      And she enters "nodes.visualops.com" in "upstream"
      And she enters "K]<n72U1y#9PUS.j9BpC=XNxz6HCqhRfnGbSnajO" in "upstreamPubKey"
      And she clicks "Ok"
      And eventually the modal dialog disappears
      Then eventually the page contains "Upstream set"
