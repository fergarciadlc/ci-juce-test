#
#  PipeApp.rb
#  Goldsmith
#
#  Created by Den Smolianiuk on 6/25/09.
#  Copyright 2014 by Avid Technology, Inc.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool

class PipeApp

  def initialize(target)
    @target = target
  end
  
  def execute(command)
    @target.send_command_to_app(command)
  end
  
  def close_app
    @target.close_app
  end
  
  def self.connection_class
    return PipeConnection
  end

end # PipeApp

end # module
