#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

class DTTMonitor

  TESTING_TIMEOUT = 30
  STATUS_QUERIES = [:available?, :ping, :get_platform,
    :get_sys_info, :get_version_info]
  OPT_FILE_NAME = Pathname.new('DigiOptionsFile.txt')
  OPT_FILE_STASH_NAME = Pathname.new('DigiOptionsFile.txt.old')
  
  def initialize(build_folder,verbose)
    @build_folder = build_folder.or_if_nil(SOURCE_DIR.parent.parent)
    @done = false
    @sys_info = SysInfo.new
    @test_in_progress = false
    @last_cmd = Time.now
    @current_master = nil
    @start_time = Time.now
    @verbose = verbose
    @app = nil
		@pid = nil
  end
  
=begin rdoc
Main event loop. Gets method names, calls them (with data), returns results to master.
=end
  def run
    LOG_MANAGER.set_verbose(false)
    LOG_MANAGER.set_log_elapsed_time(true)
    LOG_MANAGER.open_log('dtt')
    
    Thread.new { 
      $stderr.print("\n\nMonitor Server Running!  Listening on IP address ", @sys_info.ip_addr, ", port ", MONITOR_PORT, " \n\n")
      
      chars = ['-', '/', '|', '\\' ]
      index = 0
      while (not @done)
        $stderr.print("\r")
        $stderr.print(" Ctrl+C to kill server... ")
        $stderr.print(chars[index])
        $stderr.print("              ")
        index = (index + 1) % 4 
        sleep 1
      end
    }
    socket_thread = Thread.new {
      begin
        server = TCPServer.new('0.0.0.0', MONITOR_PORT)
        server.binmode if RUNTIME_INFO.win?
      rescue SystemCallError => err
        err_string = (err.errno == Errno::EADDRINUSE::Errno) ? "Another copy of Monitor is running." : err.to_s
        err_string.abort
      end
      while ((not @done) && (socket = server.accept))
        begin
          cmd = Marshal.load(socket)
          $stderr.puts(cmd.inspect) if @verbose
          if busy? && !STATUS_QUERIES.include?(cmd.id) && (@current_master != cmd.sender) && !cmd.data[:override]
            reply = :busy_signal
          else
            # added here since commands don't know the sender
            @current_master = cmd.sender if cmd.id == :start_testing
            Timeout::timeout(cmd.timeout) do
              |timeout_length|
              reply = self.send(cmd.id, cmd.data)
            end
          end
        rescue Exception => err
          reply = err
          $stderr.puts(err) if @verbose
        end
        begin
          $stderr.puts((/get_file/.match(cmd.id.to_s)) ? "FILE SENT" : reply) if @verbose
          socket.print(Marshal.dump(reply)) if !socket.nil?
          socket.close if !socket.nil?
        rescue Exception => err
          $stderr.puts(err) if @verbose
          reset
        end
        @last_cmd = Time.now if !STATUS_QUERIES.include?(cmd.id)
      end
    }
    socket_thread.join
  end

  def busy?
    return testing?
  end
  
  def available?(data)
    return !busy?
  end
   
  def set_timeout_factor(factor = 1.0)
    TEST_INFO.timeout_factor = factor
  end
  
  def start_testing(data = nil)
    "#{@sys_info.ip_addr} is busy.".abort if busy?
    @test_in_progress = true
    return true
  end
  
  def stop_testing(data = nil)
    @test_in_progress = false
    @current_master = nil
    return true
  end
  
  def get_sys_info(data)
    # Under Ruby 1.9.1, this breaks on Windows due to WIN32OLE changes.
    # TO_DO: fix it.
    #@sys_info = SysInfo.new(@sys_info.use_current_volume)
    return @sys_info
  end
  
  def ping(data)
    return :gnip
  end

  def get_platform(data)
    return RUNTIME_INFO.platform
  end

  
  def quit(data)
    @done = true
    return "Bye!"
  end

  def get_sources_folder(data)
     return SOURCE_DIR
  end

  def delete_pt_prefs(data)
    return PTPrefWrangler.delete_prefs(@sys_info)
  end

	def kill_dsh(data)
		begin
			Timeout::timeout(data[:timeout].or_if_nil(60)) do
        pr = Process.kill(9, @pid)
				pr = Process.waitpid(@pid)
			end
			return true
		rescue TimeoutError
			return false
		end
	end

  def build_folder(data)
    return Pathname.new(@build_folder)
  end

  def build_folder=(path)
    @build_folder = Pathname.new(path)
  end

  def launch_app(data = {})

    begin
      app_name = data[:app_name]
      return DTTError.new("Test tool name is missing.") if app_name.nil?
      
      app_class = data[:app_class]
      return DTTError.new("app_class is missing.") if app_class.nil?
      
      app_options = data[:app_options]
      local_target = data[:local_target]
      @app.close if (@app)
      @app = app_class.connection_class.new(app_name, app_options, @build_folder, local_target)
			@pid = @app.pid
    rescue DTTError => err
      return err
    rescue Exception => err
      return DTTError.new("Error launching app #{app_name} : #{err}.")
    end
    
    return true
  end
  
  def send_command_to_app(app_cmd)
    return @app.send_command(app_cmd)
  end
  
  def close_app(app_cmd)
    @app.close
    @app = nil
  end

  def move_pt_options(data = {})
    opt_file.rename(opt_file_stash) if opt_file.exist?
    return true
  end
  
  def restore_pt_options(data = {})
    opt_file.delete if opt_file.exist?
    opt_file_stash.rename(opt_file) if opt_file_stash.exist?
    return true
  end

  def exist_on_target?(path)
    if (path.class == String)
      return File.exists?(path)
    elsif (path.class == Pathname)
      return path.exist?
    else
      return DTTError.new("Unrecognized path class #{path.class}")
    end
  end
  
  def get_file(file_name)
    path = Pathname.new(file_name)
    contents = ""
    if path.exist?
      path.open do
        |file|
        file.binmode
        contents = file.read
      end
      return contents
    else
      return DTTError.new("Couldn't find file #{file_name}.")
    end
  end

  def delete_file(file_name)
    path = Pathname.new(file_name)
    if path.exist?
      File.delete(file_name) 
      return true
    else
      return DTTError.new("Couldn't find file #{file_name}.")
    end
  end
  
  def perform_on_target(data = {})
    result = nil
    data.each_pair {
      |k, v|
      "#{k}: #{v} (#{v.class})".log_verbose
    }
    if !(data.has_key?(:obj) && data.has_key?(:method))
      result = DTTError.new("Invalid perform spec: #{data.inspect}")
    else
      args = data[:args].or_if_nil([])
      block = data[:block].nil? ? Proc.new {} : data[:block]
      begin
        result = data[:obj].send(data[:method], *args, &block)
      rescue Exception => err
        result = err
      end
    end
    return result
  end
  
  private

  def opt_file
    return @build_folder + OPT_FILE_NAME
  end
  
  def opt_file_stash
    return @build_folder + OPT_FILE_STASH_NAME
  end

  def dsh_running?(data = nil)
    return false if @pid.nil?

    cmd_win = "tasklist /NH /FI \"PID eq #{@pid}\""
    cmd_mac = "ps -p #{@pid}"

    cmd = RUNTIME_INFO.win? ? cmd_win : cmd_mac
    return `#{cmd}`.include?(@pid.to_s)
  end

  def testing?
    return @test_in_progress && (@last_cmd.since < TESTING_TIMEOUT)
  end
  
  def test_notify(exit_val, test, log, spec)
    new_spec = spec.clone.update(
      'name' => TEST_NOTIFY_CMD,
      'exit_val' => exit_val,
      'log' => log,
      'test' => test
    )
    notify(new_spec)
  end
  
  def dl_notify(spec, err)
    succeeded = err.nil?
    err_string = succeeded ? 'none' : err.to_s
    spec.update(
      'name' => DONE_DOWNLOADING_CMD, 
      'succeeded' => succeeded, 
      'err_string' => err_string, 
      'target_addr' => RUNTIME_INFO.ip_addr
    )
    notify(spec)   
  end
  
  def notify(spec)
    if spec.has_key?('notify_addr') && spec.has_key?('notify_port')
        message = YAML.dump(spec)
        begin
          tcps = TCPSocket.new(spec['notify_addr'], spec['notify_port'])
          Timeout::timeout(30) do
            |timeout_length|
            tcps.puts(message)
          end
          tcps.close
        rescue SocketError, SystemCallError, TimeoutError, IOError => dl_notify_err
          $stderr.puts("Error communicating with download requestor #{spec['notify_addr']}: #{dl_notify_err}.")
        end
    end   
  end
  
  def reset
    @test_in_progress = @disable_indexing = false
    @current_master = nil
  end

end
