
When('there is default security') do

   step "there are users:", table(%{
         | name      | admin  | fullname  | id                       |
         | tracy     | true   | Tracy     | 6121bdfaec9e5a059715739c |
         | leanne    | false  | Leanne    | 6142d258ddf5aa5644057d35 |
   })
   
   step "there are groups:", table(%{
         | name        | id                        |
         | Team 1      | 61a58472ddf5aa463a08293d  |
   })
         
   step "there are policies:", table(%{
#         | name   | users        |
#          | p1     | leanne tracy |
         | name   | viewuser           | viewgroup | edituser           | editgroup | execuser           | execgroup |
         | p1     |                    | Team 1    |                    | Team 1    |                    | Team 1    |
   })
         
   step "there are users in group \"Team 1\":", table(%{
         | name      | roles           |
         | tracy     | admin user      |
         | leanne    | pleb user       |
   })
         
   step "the security indexes are generated"

end

