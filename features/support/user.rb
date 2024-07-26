require 'mongoid'

class User
   include Mongoid::Document

   field :name, type: String
   field :modifyDate, type: Time
   field :salt, type: String
   field :hash, type: String
   field :newHash, type: String
   field :fullname, type: String
   field :admin, type: Boolean
   
end
