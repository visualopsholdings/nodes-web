require 'mongoid'

class Idea
   include Mongoid::Document
  
   field :basedoc, type: String
   field :policy, type: String
   field :text, type: String # in message
   field :user, type: String # in message
   field :modifyDate, type: Time # in message and idea
   field :stream, type: String # in message and idea
   
end
