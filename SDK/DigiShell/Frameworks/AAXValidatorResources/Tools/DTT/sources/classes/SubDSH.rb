#
#  SubDSH.rb
#
#  Created by Yevgen Chebotarenko
#  Copyright 2019 by Avid Technology, Inc.

module DishTestTool
  require 'PipeConnection'
  require 'AppCmd'

  class SubDSH < PipeConnection
    
    def initialize(shell, local_target = true)
      @local_target = local_target
      @cmd_pipe_path = shell["cmd_pipe_path"]
      @ans_pipe_path = shell["ans_pipe_path"]

      wfd = IO.sysopen(@cmd_pipe_path, "w")
      @cmd_stream = IO.new(wfd, "w")
      @cmd_stream.sync = true

      rfd = IO.sysopen(@ans_pipe_path, "r")
      @reply_stream = IO.new(rfd, "r")

      @pipe = self
    end

    def puts(cmd)
      @cmd_stream.puts(cmd)
    end

    def readline
      @reply_stream.readline
    end

    def close
      @reply_stream.close
      @cmd_stream.close
    end

    def method_missing(sym, *args, &block)
      params = nil
      if (args.size == 1)
        params = args[0]
      elsif (args.size > 1)
        params = args
      end
      cmd = AppCmd.new(sym.to_s, params)
      cmd_output = self.send_command(cmd)
      return cmd_output.result["cmd_result"]
    end
  end
end
