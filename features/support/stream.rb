require 'mongoid'

class Stream
   include Mongoid::Document

   field :name, type: String
   field :modifyDate, type: Time
   field :policy, type: String
   field :active, type: Boolean

end
