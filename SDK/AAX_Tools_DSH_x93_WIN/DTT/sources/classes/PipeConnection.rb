#
#  PipeConnection.rb
#  Goldsmith
#
#  Created by Den Smolianiuk on 6/22/09.
#  Copyright 2014 by Avid Technology, Inc.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool

  class PipeConnection

    attr_reader :pid

    def initialize(app_name, app_options, search_path, local_target = true)

      if RUNTIME_INFO.win?
        app_name += '.exe'
      else
        search_path += 'CommandLineTools'
      end

      app_path = search_path + app_name

      raise DTTError.new("Could not find \"#{app_path}\"") unless app_path.exist?

      app_path = app_path.to_s + " " + app_options
      app_path.log_verbose

      @pipe = IO.popen(app_path, "r+")
      @pid = @pipe.pid

      @pipe.sync = true
      @local_target = local_target
    end

    def close
      @pipe.close
    end

    def send_command(app_cmd)
      cmd = app_cmd.to_yaml
      @pipe.puts(cmd)
      str =  "> " + cmd.gsub(/\n([^\n])/, "\n> \\1")
      print_string(str)

      # skip all till meet yaml start document
      yaml = ''
      begin
        line = @pipe.readline
        str = "# " + line
        print_string(str)
      end while (line !~ /^---$/)

      begin
        yaml += line

        line = @pipe.readline
        str =  "< " + line
        print_string(str)
      end while (line !~ /^\.\.\.$/)

      yamlout = YAML::load(yaml)

      return AppCmdResult.new(yamlout)
    end

    def print_string(str)
      @local_target ? str.log_dsh : puts(str)
    end
  end # PipeConnection
end # module
