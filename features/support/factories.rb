require 'factory_bot'
require 'date'

FactoryBot.define do

   factory :user do |f|
      f.modifyDate { DateTime.now }
      f.salt { 'T9Bv8J0hPaFJ02Rp22pSSt4aSTiYX7ZL3gmYF0aYFz+aLjA/Mzp26WJhf6QLuoRQrYb6GCaysCznRVNwsS13+YtvFKEQU1yII0tcTlkMIwUHUOxf8bimCyHXtr951Wi08x7iQsCNivghKyxPYlLmoymuDZ27bWYgByRG4vOGlUM=' }
      f.hash { '9TxF9r/DTwYKu1WadrlhHlnbXnAWflF5gqZ9+M0VX1Hyq8ixnle6OIkS+lm2xl6KxxW0Af9CaJNacVGuBi5Pbsj4RiSN1FrXxtKnLqKkdskNfSQnRktDzZO88iBC7kp80vscAUysH0H+U2Ihs623eEN3LY47Z5vUu+uTsN5Wxs8=' }
   end

   factory :stream do |f|
      f.modifyDate { DateTime.now }
   end

   factory :policy do |f|
      f.modifyDate { DateTime.now }
   end

   factory :access do |f|
      f.users { [] }
      f.groups { [] }
   end

   factory :group do |f|
      f.modifyDate { DateTime.now }
   end

   factory :group_member do |f|
   end

   factory :idea do |f|
      f.modifyDate { DateTime.now }
   end

end
