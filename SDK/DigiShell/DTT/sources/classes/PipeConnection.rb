#
#  PipeConnection.rb
#  Goldsmith
#
#  Created by Den Smolianiuk on 6/22/09.
#  Copyright 2022 by Avid Technology, Inc.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool
  class PipeConnection

    attr_reader :pid
    def initialize(app_name, app_options, search_path, local_target = true)
      app_name << '.exe' if RUNTIME_INFO.win?

      if RUNTIME_INFO.win?
        tool_dir_name = Pathname.new('')
      elsif RUNTIME_INFO.linux?
        tool_dir_name = Pathname.new('bin')
      else
        tool_dir_name = Pathname.new('CommandLineTools')
      end

      path = search_path + tool_dir_name
      raise DTTError.new("Tool dir not found.") unless path.exist?

      path = path + Pathname.new(app_name)
      raise DTTError.new("Test tool \"#{app_name}\" not found.")  unless path.exist?

      launch_app = "#{path} #{app_options}"
      launch_app.log_verbose

      @pipe = IO.popen(launch_app, "r+")
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

      yamlout = YAML::load_documents(yaml.gsub /\t/, '') #switched to accept several jaml responses for one command

      return AppCmdResult.new(yamlout[-1]) #always use the last command result which is usually cmd_result or cmd_error
    end

    def print_string(str)
      @local_target ? str.log_dsh : STDOUT.puts(str)
    end

  end # PipeConnection
end # module

