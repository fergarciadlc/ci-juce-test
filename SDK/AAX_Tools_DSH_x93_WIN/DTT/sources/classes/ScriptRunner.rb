#
#  ScriptRunner.rb
#  Goldsmith
#
#  Created by Tim Walters on 5/5/08.
#  Copyright 2014 by Avid Technology, Inc.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool

=begin rdoc
  Class used internally by RunSuite.
=end
  class ScriptRunner # :nodoc:

    def initialize(script, target, delete_prefs = true, debug_mode = false,
		    verbose = false, log_elapsed_time = true, simplified_output = false,
		    disable_digitrace = false)

      @script = script
      @target = target
      @delete_prefs = delete_prefs
      @debug_mode = debug_mode
      @verbose = verbose
      @log_elapsed_time = log_elapsed_time
      @simplified_output = simplified_output
      @disable_digitrace = disable_digitrace
    end
  
    def run(console_io = nil, log_dir = nil)
      LOG_MANAGER.set_verbose(@verbose)
      LOG_MANAGER.set_log_elapsed_time(@log_elapsed_time)
      log_file_path = LOG_MANAGER.open_log(@script.name, console_io, log_dir)
      if @simplified_output
        "Running #{@script.name}...".log
        LOG_MANAGER.suspend_console(true)
      end
			"Test Log: #{log_file_path}".log
      "Tester: #{RUNTIME_INFO.username}".log_verbose
      "Start time: #{Time.now}".log_verbose
  	  "======".log
      @target.sys_info.log_string.log_info
      @target.delete_pt_prefs if @delete_prefs
      result = nil
      script = @script
      script.target = @target
      start_time = Time.now
      DigiShell.disable_digitrace if @disable_digitrace
      script.attempts.times do
        |i|
        "#{@script.name} retry #{i}...".log if i > 0
        begin
          result = run_a_script(script)
        rescue Exception => err
          result = ScriptResult.new(@script.class.short_name, @target, :abort, err.to_s, {}, @script.options)
          err.backtrace.each {|s| s.log_verbose}
          if @debug_mode
            result.log
            LOG_MANAGER.write_master_log(script, start_time, result)
            raise err
          end
        end

        result.target = @target
        needs_kill = false
        begin
	        needs_kill = @target.dsh_running?
        rescue Exception => err
	        needs_kill = true
        end

        if needs_kill
	        begin
		        @target.kill_dsh
	        rescue Exception => err
		        "Failed to kill dsh process".log_status
	        end
        end

        result.add_warnings(TEST_INFO.warnings)
        TEST_INFO.clear_warnings
        if @simplified_output
          LOG_MANAGER.suspend_console(false)
          result.to_s.log_status
        else
          result.log
        end
        # Only do the master log if custom logging is not on
        LOG_MANAGER.write_master_log(script, start_time, result) if console_io.nil?
        break if result.passed?
      end
      ('Elapsed time: ' + start_time.since.elapsed_time_string).log
      LOG_MANAGER.close_log
      result.args = script.options
      result.log_file_path = log_file_path
      return result
    end
  
    private

    def run_a_script(script)
      script.dump_options.log_verbose
      result = script.run
      return result
    end

  end # Goldsmith

end # module