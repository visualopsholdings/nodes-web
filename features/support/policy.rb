require 'mongoid'

class Policy
   include Mongoid::Document

   embeds_many :accesses
  
   field :name, type: String
   field :modifyDate, type: Time
#   field :users, type: Array
      
end

class Access
   include Mongoid::Document

   embedded_in :policy
  
   field :name, type: String
   field :users, type: Array
   field :groups, type: Array
  
end
