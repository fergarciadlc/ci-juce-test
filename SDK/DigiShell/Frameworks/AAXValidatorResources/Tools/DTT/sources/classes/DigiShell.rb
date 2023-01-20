#
#  DigiShell.rb
#  Goldsmith
#
#  Created by Rus Maxham on 9/14/09.
#  Copyright 2014 by Avid Technology, Inc.

#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool#

require 'AAXHDishUtils'

module DishTestTool

=begin rdoc
Class for interacting with DigiShell from Goldsmith.  Launches dsh when created and
uses method_missing to pass most function calls on an instance of DigiShell right
through to the running executable.  For example:

  # create an instance of DigiShell, which creates an instance of dsh
  dsh = DigiShell.new(target)
  # print the results of the help command
  p dsh.help
  
=end
  
  class DigiShell
    include AAXHDishUtils

    @@disable_digitrace = false
    class Failure < Exception
      def initialize(cmd, reason)
        super("DSH command '#{cmd}' returned error: #{reason}'")
      end
    end

    class FailureException < Exception
      def initialize(cmd, exception)
        super("DSH command '#{cmd}' failed with exception:\n" + exception.to_yaml)
      end
    end

###########################################################
=begin rdoc
Constructor.  Takes as an argument an execution target host where dsh should be run.
=end

    def initialize(target, remote = false)

      @dsh = target.launch_app(PipeApp, "dsh", "-a")
      @singlestepping = false
      @singlesteppingonce = false
      @singlesteppingonerror = false
      @remote = remote
      enable_logging unless @@disable_digitrace
      if remote
        @rdsh = RemoteConnection.new(self)
        @rdsh.new_connection
      end

      self_info = self.info

      RUNTIME_INFO.dsh_log_files << self_info["trace_path"]
      RUNTIME_INFO.debug_build = :true if self_info["build"] == "debug"

    end

###########################################################
=begin rdoc
Destructor.  Closes the dsh connection.
=end

    def DigiShell.finalize(id)
      id.close
    end

    def DigiShell.disable_digitrace
      @@disable_digitrace = true
    end

###########################################################
=begin rdoc
Closes the dsh connection by sending 'exit' to dsh.
=end

    def close
      if (@dsh)
        @rdsh.disconnect if @remote
        @dsh.execute(AppCmd.new("exit", nil))
        @dsh.close_app
      end

      @dsh = nil
    end

    def closed?
      return !@dsh
    end

###########################################################
=begin rdoc
Enable DigiTrace logging
=end

    def enable_logging
      clear_trace_config
      enable_trace_facility(["DTF_GOLDSMITH", "DTP_LOW"])
      enable_trace_facility(["DTF_DSI", "DTP_NORMAL"])
      enable_trace_facility(["DTF_COREAUDIO", "DTP_NORMAL"])
      enable_trace_facility(["DTF_CORE_ENGINE_SETTINGS", "DTP_LOWEST"])
    end

###########################################################
=begin rdoc
Loads dish and checks the result. Throws exception in case of failure.
=end

    def load_dish(dish_or_dishes)
      dish_or_dishes = [dish_or_dishes] if dish_or_dishes.kind_of? String
      result = execute("load_dish", dish_or_dishes)

      if (!result.has_key?('loaded_dishes_count'))
        raise Failure.new("load_dish", "result hash doesn't contain loaded_dishes_count")
      end

      unless (result['loaded_dishes_count'] == dish_or_dishes.size)
        raise Failure.new("load_dish", "failed to load some of " + dish_or_dishes.collect{|a| a + ", "}.to_s.chop.chop)
      end
    end

###########################################################
=begin rdoc
Execute DigiShell command. Converts error messages to exceptions if any.
=end

    def execute(cmd, params)

      dsh_cmd_params = params.nil? ? "" : params.inspect.gsub(/"/, '').gsub(/=>/, ': ')
      "#{cmd} #{dsh_cmd_params}".log_dsh_cmd
      result = @dsh.execute(AppCmd.new(cmd, params)).result

      # result = hash_result[-1] #in case there're additional messages for "progress" we take only the last one which is always cmd_result or cmd_failes

      commandfailed = false
      if (result.has_key?('message_type') and result['message_type'] == 'cmd_failed')
        commandfailed = true
      end

      dostep = @singlestepping || @singlesteppingonce || (commandfailed && @singlesteppingonerror)
      @singlesteppingonce = false
      if( dostep )
        puts "dsh> #{cmd} #{params}"  #TODO: dump params as flow-style inline yaml (on single line) - can't figure out how to do that
        puts YAML::dump( result )
        self.interactive
      end

      if( commandfailed )
        if (result['cmd_failure_reason'] == 'failed_with_exception')
          result.delete('message_type')
          result.delete('cmd_failure_reason')
          raise FailureException.new(cmd, result)
        else
          raise Failure.new(cmd, result['cmd_failure_reason'])
        end
      end
      return result['cmd_result']
    end

###########################################################
=begin rdoc
Turns method calls into dsh commands.
=end

    def method_missing(sym, *args, &block)
      # this turns any method call into a command sent
      # to the shell

      params = nil
      if (args.size == 1)
        params = args[0]
      elsif (args.size > 1)
        params = args
      end

      return execute(sym.to_s, params)
    end

###########################################################
=begin rdoc
Send command into dsh.
=end

    def cmd(sym, *args)
      
      params = nil
      if (args.size == 1)
        params = args[0]
      elsif (args.size > 1)
        params = args
      end

      return execute(sym.to_s, params)
    end

###########################################################
=begin rdoc
Runs an interactive pass-thru to the dsh.  Also adds several
built-in commands for debugging.
=end

    def interactive

      continue_requested = false
      STDOUT.print "\n"
      begin
        STDOUT.print "dsh> "
        commandline = STDIN.gets
        tokens = commandline.split(' ')
        if( tokens.length > 0 )
          command = tokens[0]
          tokens.delete_at(0)  # shift tokens down
          case command
          when 'continue', 'c'
            continue_requested = true
          when 'step', 's'
            if( tokens.size == 1 )
              if( tokens[0] == 'on' )
                @singlestepping = true
                STDOUT.print "   auto single stepping enabled\n"
              elsif( tokens[0] == 'off' )
                @singlestepping = false
                @singlesteppingonerror = false
                STDOUT.print "   single stepping disabled\n"
              elsif( tokens[0] == 'error' )
                @singlesteppingonerror = true
                STDOUT.print "   single step on error enabled\n"
              else
                STDOUT.print "   unknown option; see 'help step'"
              end
            else
              @singlesteppingonce = true
              continue_requested = true
            end
          when 'exit'   # don't allow exit
            STDOUT.print "  <not allowed>\n"
          else
            params = nil
            if (tokens.size == 1)
              params = tokens[0]
            elsif (tokens.size > 1)
              paramstring = commandline.sub( command, "" )
              params = YAML.load( paramstring )
            end
            result = @dsh.execute(AppCmd.new(command, params)).result
            if( command == 'help' && tokens.size == 0)
              result[ "cmd_result" ].push( "continue, c: continue script execution" )
              result[ "cmd_result" ].push( "step, s: [on | off | error] continue script execution, stopping at next dsh command, optionally always stopping or stopping on error" )
            end
            puts YAML::dump( result )
          end
        end
      end while continue_requested == false
    end

  end # DigiShell

end # module
