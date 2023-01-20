#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool

=begin rdoc
The class that runs scripts and suites. Used by runsuite.rb and sometimes by gst.rb.
=end
  class RunSuite
=begin rdoc
    Sets up the run.
    +suite_or_script+: a bit overloaded... should be a list of hashes, each 
    representing a script to be run and its arguments, _or_ a path to a YAML suite file (.gss),
    _or_ the name of a script (subclass of Script), with the arguments to follow in +args+.
    +targets+: a Target object for the scripts
    +delete_prefs+: true or false, deletes PT prefs if true
    +debug_mode+: true or false, will stop on any error, without cleaning up target, if true
    +lenient+: true or false, silently skips invalid scripts if true
    +verbose+: true or false, logs verbose output to console if true
    +log_elapsed_time+: true or false, does what it says on the tin
    +move_options+: true or false, moves PTOptions file if true
    +format_output_for_gui+: true or false, determines style of master log. Default false, GUI sets it to true
    +args+: args for script in single-script mode
    +on_startup+: true or false, sets some values needed for autosmoke
=end
    def initialize(suite_or_script, target, delete_prefs = true, debug_mode = false,
                   verbose = false, log_elapsed_time = false, move_options = true,
                   args = {}, simplified_output = false, disable_digitrace = false,
                   pb_port = PROTOBUFF_PORT, zeta_device_logs = false, ktrace_logs = false, export_logs = false)

      @scripts = []
      @results = []
      @target = target
      @delete_prefs = delete_prefs
      @debug_mode = debug_mode
      @verbose = verbose
      @log_elapsed_time = log_elapsed_time
      @move_options = move_options
      @simplified_output = simplified_output
      @disable_digitrace = disable_digitrace
      @zeta_device_logs = zeta_device_logs
      @ktrace_logs = ktrace_logs
      @export_logs = export_logs
      @suite_result = SuiteResult.new(export_logs)
      @suite_result.pb_port = pb_port
      @global_result = Array.new


      suite_or_script = [suite_or_script] if suite_or_script.class != Array

      @scripts = Array.new
      suite_or_script.each do |item|
        next if (item.instance_of?(Hash) and !item['enabled'])

        script = {:name => nil, :args => {}}
        common_args = args.dup

        if item.instance_of?(Hash)
          script[:name] = item['name']
          script[:args] = (item['args'].nil?) ? common_args : common_args.merge(item['args'])
        else
          script[:name] = item
          script[:args] = args
        end

        @scripts.push(script)
      end
    end

=begin rdoc
    Pull the trigger. Passes args to ScriptRunner, q.v.
=end
    def run(console_io = nil, log_dir = nil)
      valid_scripts = Array.new

      # Add to RUNTIME_INFO true if running a Debug build, false for Release build.
      if @target.win?
        pattern = /Debug\/bin$/i
      else
        pattern = /Debug/i
      end
      build_folder = @target.build_folder.to_s
      RUNTIME_INFO.debug_build = !(build_folder =~ pattern).nil?
      
      # Run test scripts
      @scripts.each do |script|
        abridged_args = script[:args].select{ |k,v| v == "all_#{k}" }
        if !abridged_args.empty?
          script_class = RUNTIME_INFO.get_script_class_by_name(script[:name])

          if script_class.nil?
            description =  "Could NOT create args combinations for unknown script: #{script[:name]}"
            @suite_result.add_script_result(ScriptResult.new(script[:name], @target, :cancel, description, {}, script[:args]))
            next
          else
            valid_args = script_class.valid_options.select{|k,v| abridged_args.include?(k)}
            list_variatons_args = expand_args(script[:args].update(valid_args))
            list_variatons_args.each do  |arg|
              script_instance = script_class.new(arg)
              script_instance.target = @target
              valid_scripts.push(script_instance)
            end
          end
        else
          script_class = RUNTIME_INFO.get_script_class_by_name(script[:name])
          if script_class.nil?
            description =  "Unknown script: #{script[:name]}"
            
            # Print all available test script names if user entered a single non-existent name
            if @scripts.count <= 1
              puts "List of existing scripts:"
              puts TEST_SCRIPTS.to_a.collect{|script| script.to_s}.sort       
            end
              
            @suite_result.add_script_result(ScriptResult.new(script[:name], @target, :cancel, description, {}, script[:args]))
          else
            script_instance = script_class.new(script[:args])
            script_instance.target = @target
            begin
              script_instance.validate
            rescue Exception => e
              description = "#{script_instance.name} cannot be run because of error: #{e.message}"
              @suite_result.add_script_result(ScriptResult.new(script[:name], @target, :cancel, description, {}, script[:args]))
              next
            end
            valid_scripts.push(script_instance)
          end
        end
      end

      valid_scripts.each{|s|("Script to Execute: " + s.name).log}
      uniq_id = valid_scripts.first.options[:uniq_id] unless valid_scripts.empty?

      if uniq_id
        if !Module.const_defined?(:AAXValResult)
          @suite_result.suite_errors.push("Please install 'ruby-protocol-buffers' gem OR run test without '-b' option")
        else
          @suite_result.init_protobuff_result(uniq_id.to_i)
        end
      end

      begin
        @suite_result.suite_errors.push("Target #{@target.addr} is busy.") if @target.busy?
        @target.start_testing
        @target.move_pt_options if @move_options
      rescue Exception => e
        @suite_result.suite_errors.push(e.message)
      end

      if @suite_result.suite_errors.size > 0
        @suite_result.suite_result = SuiteResult::CANCELED
        @suite_result.write_result
        return @suite_result.suite_result
      end

      begin
        valid_scripts.each do |s|

          result = ScriptRunner.new(s, @target, @delete_prefs, @debug_mode, @verbose, @log_elapsed_time,
                                    @simplified_output, @disable_digitrace, @zeta_device_logs, @ktrace_logs, @export_logs).run(console_io, log_dir)
          
          @global_result << result.result
          @suite_result.add_script_result(result)
        end
        
        #Added section to set global suite result
        if @global_result.all? { |state| state == :pass } && @global_result.size > 0
          @suite_result.suite_result = SuiteResult::PASSED
          else  
          @suite_result.suite_result = SuiteResult::FAILED
        end
        #########################################
        
        @target.restore_pt_options if @move_options

      rescue SignalException, SystemExit => e
        @suite_result.add_suite_error("Runsuite terminated. Notifying target...")
        @suite_result.suite_result = SuiteResult::ABORTED
        @suite_result.backtrace(e.backtrace)
      rescue Exception => e
        @suite_result.add_suite_error("Runsuite error: #{e.message}")
        @suite_result.suite_result = SuiteResult::RESULTSTATUS_UNKNOWN
        @suite_result.backtrace(e.backtrace)
      ensure
        @target.stop_testing
      end

      @suite_result.suite_result = SuiteResult::LOST if @suite_result.suite_result.nil?

      @suite_result.write_result
      return @suite_result.suite_result
    end

    private
    def expand_args(args = {})
      arguments = []
      args.each{|key,v|
        tmp = []
        values = v.instance_of?(Array) ? v : [v]
        values.each {|value| tmp.push({key => value})}
        arguments.push(tmp)
      }

      return variations(arguments)
    end

    def variations(list)
      first = list.first
      if list.size == 1
        first
      else
        rest = variations(list[1..-1])
        first.map { |x| rest.map {|y| x.merge(y)}}.flatten
      end
    end

    def validate_script(script)

    end

  end
end
