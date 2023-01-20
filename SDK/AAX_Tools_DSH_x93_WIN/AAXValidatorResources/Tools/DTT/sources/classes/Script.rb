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
			return self.inputs.keys.sort
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
				when FalseClass, TrueClass
					raw_arg
				when NilClass
					"Invalid script spec: default cannot be nil.".abort(self)
				else
					default.class.newFromDisplay(raw_arg)
			end
		end
	end # Script
end # module
