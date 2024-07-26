require 'mongoid'

class Group
   include Mongoid::Document

   store_in collection: "groups"
   
   field :name, type: String
   field :modifyDate, type: Time
   field :policy, type: String
   field :image, type: String
   field :upstream, type: Boolean
   
   embeds_many :group_members, store_as: "members"

end

class GroupMember
   include Mongoid::Document
  
   embedded_in :group

   field :user, type: String
   field :roles, type: Array

end
