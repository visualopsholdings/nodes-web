
Given(/^there are users:$/) do |users|

   users.hashes.each do |s|
   
      user = FactoryBot.build(:user)
      user.name = s[:name]
      user.admin = s[:admin]
      
      if s[:id] && s[:id].length > 0
         user._id = s[:id]
      end
      
      if s[:fullname] && s[:fullname].length > 0
         user.fullname = s[:fullname]
      end
            
      user.save
      
   end
   
end

Given(/^there are policies:$/) do |table|

   table.hashes.each do |s|
      policy = FactoryBot.build(:policy)
      policy.name = s[:name]
      
      if s[:id] && s[:id].length > 0
         policy._id = s[:id]
      end
      
#       if s[:users] && s[:users].length > 0
#          policy.users = []
#          s[:users].split(" ").each do |u|
#             policy.users.push(User.where(name: u).first._id.to_s)
#          end
#       end
      
      access = FactoryBot.build(:access)
      access.name = 'view'
      if s[:viewuser] && s[:viewuser].length > 0
         s[:viewuser].split(" ").each do |u|
            access.users << User.where(name: u).first._id.to_s
         end
      end
      if s[:viewgroup] && s[:viewgroup].length > 0
         s[:viewgroup].split(";").each do |g|
            access.groups << Group.where(name: g).first._id.to_s
         end
      end
      policy.accesses << access
      
      access = FactoryBot.build(:access)
      access.name = 'edit'
      if s[:edituser] && s[:edituser].length > 0
         s[:edituser].split(" ").each do |u|
            access.users << User.where(name: u).first._id.to_s
         end
      end
      if s[:editgroup] && s[:editgroup].length > 0
         s[:editgroup].split(";").each do |g|
            access.groups << Group.where(name: g).first._id.to_s
         end
      end
      policy.accesses << access

      access = FactoryBot.build(:access)
      access.name = 'exec'
      if s[:execuser] && s[:execuser].length > 0
         s[:execuser].split(" ").each do |u|
            access.users << User.where(name: u).first._id.to_s
         end
      end
      if s[:execgroup] && s[:execgroup].length > 0
         s[:execgroup].split(";").each do |g|
            access.groups << Group.where(name: g).first._id.to_s
         end
      end
      policy.accesses << access
      
      policy.save
   end
   
end

Given(/^there are streams:$/) do |table|

   table.hashes.each do |s|
      stream = FactoryBot.build(:stream)
      
      if s[:id] && s[:id].length > 0
         stream._id = s[:id]
      end
      
      if s[:modifyDate] && s[:modifyDate].length > 0
         stream.modifyDate = parse_date(s[:modifyDate])
      end

      stream.name = s[:name]

      if s[:policy] && s[:policy].length > 0
         stream.policy = Policy.where(name: s[:policy]).first._id.to_s
      end

      stream.active = true
      stream.save
   end
end


When('there are groups:') do |table|

   table.hashes.each do |s|
      group = FactoryBot.build(:group)
      
      if s[:id] && s[:id].length > 0
         group._id = s[:id]
      end
      
      if s[:modifyDate] && s[:modifyDate].length > 0
         group.modifyDate = parse_date(s[:modifyDate])
      end

      group.name = s[:name]
      group.save
   end

end

When('there are users in group {string}:') do |name, table|

   group = Group.where(name: name).first
   table.hashes.each do |s|
      user = User.where(name: s[:name]).first
      member = FactoryBot.build(:group_member)
      member.user = user._id.to_s
      member.roles = []
      if s[:roles] && s[:roles].length > 0
         s[:roles].split(" ").each do |r|
            member.roles.push(r)
         end
      end
      group.group_members << member
   end
   group.save
   
   puts `$NODES_HOME/build/nodesaggregate --coll=group $NODES_HOME/src/useringroups.json`

end

When('the security indexes are generated') do

   puts `$NODES_HOME/build/nodesaggregate --coll=policy $NODES_HOME/src/groupeditpermissions.json`
   puts `$NODES_HOME/build/nodesaggregate --coll=policy $NODES_HOME/src/groupviewpermissions.json`
   puts `$NODES_HOME/build/nodesaggregate --coll=policy $NODES_HOME/src/usereditpermissions.json`
   puts `$NODES_HOME/build/nodesaggregate --coll=policy $NODES_HOME/src/userviewpermissions.json`

end

When('there are ideas:') do |ideas|

   ideas.hashes.each do |s|
      idea = FactoryBot.build(:idea)
      idea.text = s[:name]
      idea.policy = Policy.where(name: s[:policy]).first._id.to_s
      if s[:by] && s[:by].length > 0
         idea.user = User.where(name: s[:by]).first._id.to_s
      end
      idea.stream = Stream.where(name: s[:stream]).first._id.to_s
      if s[:modifyDate] && s[:modifyDate].length > 0
         idea.modifyDate = parse_date(s[:modifyDate])
      end
      idea.save
   end

end
