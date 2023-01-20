#
#   RemoteDSH.rb
#
#   Created by Yevgen Chebotarenko
#   Reworked by Sergii Anisienko  
#
#   Copyright (c) 2020 Avid Technology. All rights reserved.
#
module DishTestTool
  class RemoteDSH

    CFG_FILE = "ZetaRemoteMachine"
    ESCAPE_CHAR = 'q'

    attr_reader :host

    #Constructor
    def initialize(user: get_config["user"], host: get_config["host"], app_path: get_config["app_path"], connection_type: get_config["connection_type"])
      @host = host
      @user = user
      @app_path = app_path
      @connection_type = connection_type
    end
    
    #Starts dsh on remote machine
    def start(host = @host)
      str = @connection_type + " -e #{ESCAPE_CHAR} " + host
      @pipe = IO.popen(str, "r+")
      check_output("Connected")
      @pipe.puts(@user, @app_path)
      "Connected to #{@host}".log_status
    end

    def start_ssh(host = @host)
      str = @connection_type + " -e #{ESCAPE_CHAR} " + "-tt " + @user + "@" + host
      @pipe = IO.popen(str, "r+")
      check_output("#")
      @pipe.puts(@app_path)
      "Connected to #{@host}".log_status
    end

    def start_sftp(host = @host)
      str = @connection_type + " " + @user + "@" + host
      @pipe = IO.popen(str, "r+")
      @pipe.puts(@app_path)
      check_output("sftp")
      @pipe.puts(@app_path)
      "Connected to #{@host}".log_status
    end
    
    #Closes connection, uses escape character for correct closing
    def close
      self.exit
      for i in 0..1
        @pipe.puts(ESCAPE_CHAR)
        sleep 0.1
      end
      @pipe.close

      return @pipe.closed?
    end
    
    def close_ssh
      self.exit

      @pipe.print(ESCAPE_CHAR,".")
      sleep 0.1
      @pipe.close

      return @pipe.closed?
    end
    
    def close_sftp
#      self.exit

      @pipe.puts("bye")
      sleep 0.1
      @pipe.close

      return @pipe.closed?
    end

    #Sends commands and receives output from dsh on remote machine
    def method_missing(sym, *args, &block)
      params = nil
      if (args.size == 1)
        params = args[0]
      elsif (args.size > 1)
        params = args
      end

      @pipe.puts("#{sym.to_s} #{params}")
      yaml = ''
      begin
        line = @pipe.readline
      end while (line !~ /^---\r/)

      begin
        yaml += line
        line = @pipe.readline
      end while (line !~ /^\.\.\.\r/)

      return AppCmdResult.new(YAML::load(yaml)).result["cmd_result"]
    end

    #Keyword based check
    def check_output(expected_output, timeout = 3.0, timeout_step = 0.01)
      output = Array.new
      loop do |res|
        i = 0
        begin
          character = @pipe.read_nonblock(1)
          output << character
        rescue IO::WaitReadable
          i+=timeout_step
          sleep timeout_step
          i < timeout ? retry : break
        end while true

        res = output.join('')
        output.clear
        if res.include?(expected_output)
          break
        else
          raise("Could not get expected output from #{@host}: #{expected_output}")
        end
      end
    end

    #Gets params from appropriate config file
    #TODO: remove when we'll have an ability to read IP directly from device using dsh command
    def get_config
      config = nil
      file = CFG_DIR + "#{CFG_FILE.to_s}.yaml"
      if File.exists?(file)
        return File.open(file){|f| config = YAML::load(f.read)}
      else
        raise "No config file found"
      end
    end

  end
end
