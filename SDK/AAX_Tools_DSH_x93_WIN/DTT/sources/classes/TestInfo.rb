#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

module DishTestTool
  
  class TestInfo
    
    attr_reader :timeout_factor, :test_target, :warnings, :non_fatal_errors, :kills
    attr_writer :timeout_factor, :test_target
    
    def initialize(info = {})
      @timeout_factor = info[:timeout_factor].or_if_nil(1.0)
      @test_target = info[:test_target].or_if_nil('localhost')
      @warnings = []
      @non_fatal_errors = 0
      @kills = 0
    end
    
    def add_warning(text)
      @warnings.push(text)
    end
    
    def clear_warnings
      @warnings = []
    end
    
    def add_non_fatal_error
      @non_fatal_errors += 1
    end
    
    def add_kill
      @kills += 1
    end
    
    def reset_kills
      @kills = 0
    end
    
  end

end