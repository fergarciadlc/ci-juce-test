#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

module DishTestTool

=begin rdoc
  Singleton class for managing logging. Created at launch in global
  variable LOG_MANAGER.
=end
  class LogManager

    attr_reader :log_elapsed_time
  
    def initialize
      LOG_DIR.mkdir if not LOG_DIR.exist?
      @logger = nil
      @verbose = false
      @warning_level = $VERBOSE
      @log_elapsed_time = true
    end
  
=begin rdoc
Logs +text+ to standard and verbose log files, and to console. Log
must be open.
=end
    def log(text, options = {})
      if check_log
        case options[:style]
        when :verbose
          @logger.log_verbose(text)
        when :info
          @logger.log_info(text)
        when :dsh
          @logger.log_dsh(text)
        when :status
          @logger.log_status(text)
        when :dsh_cmd
          @logger.log_dsh_cmd(text)
        else
          @logger.log(text)
        end
      else
        puts text
      end
    end
  
=begin rdoc
  Called at end of script. Don't call manually.
=end
    def write_master_log(script, start_time, result)
      "Can't write master log; no logger active.".abort if @logger.nil?
      @logger.write_master_log(script, start_time, result)   
    end
  
=begin rdoc
Logs +text+ to verbose log files, and to console if verbose is set. Log
must be open.
=end
    def log_verbose(text, options = {})
      log(text, options.merge( :style => :verbose ))
    end

=begin rdoc
Logs +text+ to info file.
=end
    def log_info(text, options = {})
      log(text, options.merge( :style => :info ))
    end

    def log_dsh(text, options = {})
      log(text, options.merge( :style => :dsh) )
    end

    def log_status(text, options = {})
      log(text, options.merge( :style => :status))
    end

    def log_dsh_cmd(text, option = {})
      log(text, option.merge( :style => :dsh_cmd))
    end

=begin rdoc
Logs text with a warning flag, and increments warning count.
=end
    def warn(text, options = {})
      TEST_INFO.add_warning(text)
      log("WARNING: " + text, options)
    end
  
=begin rdoc
Redirects all console i/o from stdout, or not
=end
    def suspend_console(suspend)
      @logger.suspend_console(suspend)
    end

=begin rdoc
Logs text with an error flag, and increments non-fatal-error count. Use when you want a
strong warning, but are willing to roll the dice and continue without throwing a GoldsmithError.
=end
    def non_fatal_error(text, options = {})
      TEST_INFO.add_non_fatal_error
      log("ERROR: " + text, options)
    end
  
=begin rdoc
Closes log files.
=end
    def close_log
      "Closing non-existent log.".abort(self) if !check_log
      @logger.close
      @logger = nil
    end
  
=begin rdoc
Opens log files. Must be called before logging. Returns
path to main log file.
=end
    def open_log(root, console_io = nil, log_dir = nil)
      if not @logger.nil? then
        close_log
      end
      this_log_dir = log_dir.or_if_nil(LOG_DIR + Pathname.new(root + '_' + TIME_INFO.time_stamp_plus()))
      if this_log_dir.exist? then
        FileUtils.rm_rf(this_log_dir)
      end
      @logger = Logger.new(this_log_dir, @verbose, @log_elapsed_time, console_io)
      return @logger.get_log_file_path(:log)
    end
  
=begin rdoc
Sets verbosity of standard log file and console. Verbose log
is always verbose.
=end
    def set_verbose(is_verbose)
      @verbose = is_verbose
      @logger.verbose = is_verbose if !@logger.nil?
    end
  
    def set_log_elapsed_time(log_et)
      @log_elapsed_time = log_et
      @logger.log_elapsed_time = log_et if !@logger.nil?
    end

    def logging?
      return !@logger.nil?
    end

    def suppress_warnings
      $VERBOSE = nil
    end
  
    def restore_warnings
      $VERBOSE = @warning_level
    end

=begin rdoc
  Runs the supplied block, logs the time taken, and returns the block's result.
=end
    def time_block(block_name)
      "Starting #{block_name}...".log
      start_time = Time.now
      result = yield
      "Finished #{block_name}, elapsed time #{start_time.since.elapsed_time_string}.".log
      return result
    end

    def log_dir
      return @logger.log_dir
    end

    private

    def check_log
      return !@logger.nil?
    end
    
  end  
end