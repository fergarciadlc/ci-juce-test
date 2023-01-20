#
#  AppCmd.rb
#  Goldsmith
#
#  Created by Den Smolianiuk on 6/22/09.
#  Copyright 2014 by Avid Technology, Inc.
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool

=begin rdoc
  Abstract class representing command sent to application under test. TODO: refactor PTCmd
  to inherit from this (probably after we switch PT communication to YAML).
=end
class AppCmd
  
  def initialize(cmd, params)
    @cmd = cmd
    @params = params
  end

  def to_yaml
    result = @cmd.to_s + "\n" + YAML::dump(@params)
    result += "...\n" unless result[/\.\.\.\n$/]
    return result
  end
  
end # AppCmd

class AppCmdResult
  
  attr_reader :result
  
  def initialize(result)
    @result = result
  end
  
end # AppCmdResult

end # module
