#
#  DTTError.rb
#  DishTestTool
#
#  Created by Tim Walters on 4/24/08.
#  Copyright 2014 by Avid Technology, Inc.

#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool#

module DishTestTool

=begin rdoc
  Error class for Goldsmith-specific errors. Thrown by String#abort,
  and halts script operation. If there's an error or set of errors you 
  regularly need to trap and continue, you should make a subclass to hold
  those errors, so that errors you didn't expect are not trapped by mistake.
  For an example, see PerformanceFailure.
=end
class DTTError < StandardError
end

=begin rdoc
  Special error class to allow nesting of errors. Should only
  be thrown when master-target connection times out.
=end
class DTTTargetTimeoutError < StandardError
end

end
