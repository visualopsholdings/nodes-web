
require 'date'

def parse_date(s)
  today = Date.today
  if s && s.start_with?("now")
    offs = s[3..-1]
    sign = '+'
    if offs[0] == '-'
       sign = '-'
       offs = offs[1..-1]
    end
    units = offs[-1]
    offs = offs[0..-2].to_i
    case units
    when 'd'
       if sign == '+'
          return today + offs.days
       else
          return today - offs.days
       end
    when 'h'
       if sign == '+'
          return today + offs.hours
       else
          return today - offs.hours
       end
    when 'm'
       if sign == '+'
          return today + offs.minutes
       else
          return today - offs.minutes
       end
    else
      raise "unknown units", units
    end
  else
   if s
    return Date.parse(s)
   end
  end
  return today
   return s
end
