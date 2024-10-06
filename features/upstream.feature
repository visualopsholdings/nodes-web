@upstream
Feature: Upstream Test

   Background:
      When there are users:
         | name      | admin  | fullname     | uibits    | id                       | modifyDate   |
         | admin     | true   | Admin        | 293853    | 56348765b4d6976b478e1bd7 | Dec 4 2021    |
         | public    | false  | Public       | 7340036   | 5ed64555155a1f6043b954e7 | Apr 8 2021    |
         | joe       | true   | Joe Bloggs   | 293853    |                          |  |
         | fred      | false  | Fred Bloggs  | 293853    |                          |  |
          
      And there are groups:
         | name        | id                        | modifyDate   |
         | Team 1      | 613d8641ec9e5a6c4785d6d2  | 2 Jan 2022   |
         
      And there are users in group "Team 1":
         | name      | roles          |
         | joe      | admin user     |
         | fred      | pleb user      |
         | admin     |                |
         
       And there are policies:
         | name   | viewuser           | viewgroup | edituser           | editgroup | execuser           | execgroup | id                       | 
         | p1     |                    | Team 1    |                    | Team 1    |                    | Team 1    | 6386dbabddf5aaf74ca1e1f6 |
         
      And groups have policies:
         | name         | policy |
         | Team 1       | p1     |
 
      And there are streams:
         | name            | policy | id                       | modifyDate   |
         | Stream 1        | p1     | 613ee472ec9e5aafc85e1301 | 4 Jan 2022   |
         | Stream 2        | p1     | 61444c6addf5aaa6a02e05b7 | 5 Jan 2022   |
         | Shared Stream   | p1     | 61a0b4de98499a20f0768351 | 2 Jan 2022   |
         | Shared 2 Stream | p1     | 63c0c08e337cce14a54e0517 | 2 Jan 2022   |

      And the server has id "4ecaf2b6-42f2-49fe-b0ef-123758a3a4db"
      And the server has privateKey "t5R[&?W.8-mjIc]cs#<T(5AW7sCJYHo^<2k-]!2!"
      And the server has pubKey "31r]#CW@p}=]xYRwvg+P>NrDffBG}wXL0%t:[j6("
      
      And the server has nodes:
         | id                                   | pubKey                                   |
         | 28d243ab-505e-4d81-a3e9-c314a8ef9761 | PUYdHOr65gaR*oW6^XxaCIR3-VN=x4W{nDlAfP-7 |
         
      And there are sites:
         | name         | headerTitle  | streamBgColor   |
         | localhost    | Upstream     | lightblue       |

   @javascript
	Scenario: Setup upstream server
      Then eventually there are 4 streams in the DB	
   