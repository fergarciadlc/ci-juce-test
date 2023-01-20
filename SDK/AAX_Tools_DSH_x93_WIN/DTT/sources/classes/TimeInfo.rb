#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

module DishTestTool
  
=begin rdoc
  Utilities for managing time logging.
=end
  class TimeInfo
    
    def initialize
      @start_time = Time.now
      @last_time_stamp = @start_time
      @last_id = 0
    end
    
=begin rdoc
  Returns date/time stamp in format YYYYMMDD_HHMMSS.
=end
    def time_stamp
      update
      return @last_time_stamp.strftime("%Y%m%d_%H%M%S")
    end
  
=begin rdoc
    Returns date/time in a bracketed, human-readable format.
=end
    def time_log
      update
      return @last_time_stamp.strftime("[%c]")       
    end
    
=begin rdoc
Returns date/time stamp with instance ID at end, for when you're doing
subsecond stuff. Use this for log file names.
=end
    def time_stamp_plus
      return time_stamp + '_' + next_id
    end
    
private
    
    def update
      ts = Time.now
      if ts > @last_time_stamp
        @last_time_stamp = ts
        @last_id = 0
      end      
    end
    
    def next_id
      @last_id += 1
      return sprintf("%04d", @last_id)
    end

  end
end