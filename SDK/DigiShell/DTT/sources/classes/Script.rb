#
#  Script.rb
#  Goldsmith
#
#  Created by Tim Walters on 5/1/08.
#  Copyright 2014 by Avid Technology, Inc.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool


module DishTestTool

=begin rdoc
Class from which all Goldsmith scripts should inherit. All scripts should
override Script#run; scripts that you want to show up in the GUI must also
override Script#inputs and Script#gui_order. Override Script#pre_flight if
you want to check some condition before running. Your script will be called with
a list of target (use Script#target when only one is desired), a hash of
argument/value pairs, and a number of desired attempts.
=end
	class Script

		DEFAULT_PASS_STRING = "another tiny step toward world audio domination."
		attr_reader :options, :name, :attempts, :sub_test_results
		attr_writer :target

		def initialize(options, attempts = 1)
			@target = nil
			@options = options
			@attempts = attempts
			@name = self.class.short_name
      @sub_test_results = []
		end

=begin rdoc
Determines the arguments (and allowed values) that will show up in the GUI.
Override it in your script to return a hash with entries in the following
format:
_:arg_name => [default_value, arg_type, post_proc]_
+arg_type+ is the class to which the argument will be assigned, e.g. TrackFormat
  or Integer.
+post_proc+ is an optional proc for post-processing the argument, e.g.
+proc { |x| x.respace }+.
=end
		def self.inputs
			return {}
		end

=begin rdoc
Determines the order of arg names in the GUI. Override it in your script
to return an array of symbols, which should be the keys of the hash
returned by Script#inputs, if you don't want the default order.
=end
		def self.gui_order
			return self.inputs.keys.collect{|sym| sym.to_s}.sort.collect{|str| str.to_sym}
		end

=begin rdoc
Determines the list items to include in log name
=end
    def self.log_annexes
      return []
    end

=begin rdoc
  Returns a string of instructions for using the script that will be displayed in the GUI.
=end
		def self.instructions
			return "No special instructions for this script."
		end

		def self.valid_options
			options = Hash.new
			self.inputs.each_pair { |arg_name, spec|
				default, valid_options, post_proc = spec
				options[arg_name] = valid_options unless valid_options.nil?
			}
			return options
		end

=begin rdoc
Used at runtime. Do not override.
=end
		def validate
			self.class.inputs.each_pair do
			|arg_name, spec|
				default, valid_options, post_proc = spec
				if @options[arg_name].nil?
					@options[arg_name] = default
				else
					@options[arg_name] = reconstruct_arg(@options[arg_name], default)
				end
				@options[arg_name] = post_proc.nil? ? @options[arg_name] : post_proc.call(@options[arg_name])
			end
			return self
    end

    def generate_annex
      annex = ''

      inputs_for_annex = self.class.inputs.select {|k,v| self.class.log_annexes.include?(k)}
      inputs_for_annex.each_pair do |arg_name, spec|
        default, valid_options, post_proc = spec
        annex +=  @options[arg_name].nil? ?  format_inputs(default) :  format_inputs(@options[arg_name])
      end
      return annex
    end

    def format_inputs(default)
      string =  case default
                  when Hash
                    default.values.join
                  when Array
                    default.join
                  else
                    default.to_s
                end

      string = "_#{string}" unless string.empty?
      return string.gsub(/\s/, '')
    end

=begin rdoc
Returns the first target in the list. Use for convenience in the usual case where
there is only one target.
=end
		def target
			return @target
		end

=begin rdoc
This is the method that will be called to run your script. Override it to
do whatever the script does. Return either Script#pass or Script#fail.
=end
		def run
		end

=begin rdoc
Call in conjunction with respond_to?
=end
		def is_a_dtt_script?
			return true
		end

=begin rdoc
Used by runsuite. Do not override.
=end
		def self.is_a_dtt_script_class?
			return true
		end

=begin rdoc
This allows you to refer to arguments in the body of a script as
if they were methods; +num_tracks+ instead of options[:num_tracks].
=end
		def method_missing(symbol, *args)
			if @options.include?(symbol) && args.empty?
				return @options[symbol]
			else
				raise(NoMethodError, "undefined method \'#{symbol}\' for #{self.class}")
			end
		end

=begin rdoc
This allows you to print and log hash values in a convenient and readable way
=end
        def p_hash(hash, level = 0, indent = 2)
          unique_values_count = 0
          hash.each do |k, v|
            (level * indent).times { print ' ' }
            print "#{k}:"
            if v.is_a?(Hash)
              puts
              unique_values_count += call(v, level: level + 1, indent: indent)
            else
              puts " #{v}"
              unique_values_count += 1
            end
          end
          unique_values_count
        end
		
=begin rdoc
Used by runsuite. Do not override.
=end
		def dump_options
			return @options.keys.collect { |key| "#{key}: #{self.send(key)}" }.join("\n")
		end

=begin rdoc
Creates a passing result to be returned from Script#run or Script#pre_flight. 
=end
		def pass(result_string = nil, data = {})
			return ScriptResult.new(@name, @target, :pass, result_string.or_if_nil(DEFAULT_PASS_STRING), data, @options, @sub_test_results)
		end

=begin rdoc
Creates a failing result to be returned from Script#run.
=end
		def fail(result_string = "", data = {})
			return ScriptResult.new(@name, @target, :fail, result_string, data, @options, @sub_test_results)
		end

=begin rdoc
Creates a cancelled result to be returned from Script#pre_flight.
=end
		def cancel(result_string = "", data = {})
			return ScriptResult.new(@name, @target, :cancel, result_string, data, @options, @sub_test_results)
    end

=begin rdoc
Creates a cancelled result to be returned from Script#pre_flight.
=end
    def abort(result_string = "", data = {})
      return ScriptResult.new(@name, @target, :abort, result_string, data, @options, @sub_test_results)
    end

		def get_test_framework
			return :DTT
    end

    def create_sub_result
      sub_result = SingleTestResult.new
      @sub_test_results.push(sub_result)

      return sub_result
	  end
  
=begin rdoc 
Starts running ktrace process in a loop. Default buffer_size = 2048.
Other options are 1024, 2048, 4096, 8192
=end
    def self.start_ktrace(buffer_size = 2048, logging_on = true)
      path = get_params
      system("echo #{TEST_INFO.user_pwd} | sudo -S #{path} -m start -b #{buffer_size} -l #{logging_on}")
    end		

=begin rdoc
Captures ktrace. Creates folder with timestamp name on the desktop.
Takes seveal minutes. If sys_diag is true captures apple system
diagnostic as well. Then symbolicates and ZIPs folder.
=end
    def self.capture_ktrace(sys_diag = true, logging_on = true)
      path = get_params
      system("echo #{TEST_INFO.user_pwd} | sudo -S #{path} -m capture -d #{sys_diag} -l #{logging_on}")
    end		

=begin rdoc
Resets ktrace 
=end
    def self.reset_ktrace(logging_on = true)
      path = get_params
      system("echo #{TEST_INFO.user_pwd} | sudo -S #{path} -m reset -l #{logging_on}")
    end	
    
=begin rdoc 
Gets config data from KtraceConfig.yaml
=end
    def self.get_ktrace_config
      file_path = Pathname(Dir.pwd).to_s + '/sources/machine_cfg/KtraceConfig.yaml'
      if File.exists?(file_path)
        YAML.load(File.read(file_path))
      else
        raise "No config file found"
      end
    end

=begin rdoc
Parses ktrace params, gets absolute path to KTrace.sh script
=end
    def self.get_params
      ktrace_config = get_ktrace_config
      path = ktrace_config["path"]
      abs_path = Pathname(Dir.pwd).to_s + path
      return abs_path
    end

		private

		def reconstruct_arg(raw_arg, default)

			# We need this kludge because Integer, Float, and Symbol don't respond to .new, for
			# some reason.
			# Ruby has a "special case" for selecting by class; instead of calling .class,
			# like you'd expect, you just case on the object. Way too helpful.
			case default
				when Fixnum, Integer, Bignum
					Integer(raw_arg)
				when Float
					Float(raw_arg)
				when Symbol
					raw_arg.to_sym
				when String
					raw_arg
				when Array
					raw_arg.split(',').collect { |ra| reconstruct_arg(ra, default[0]) }
				when FalseClass, TrueClass, NilClass
					raw_arg
				else
					default.class.newFromDisplay(raw_arg)
			end
		end
	end # Script
end # module
