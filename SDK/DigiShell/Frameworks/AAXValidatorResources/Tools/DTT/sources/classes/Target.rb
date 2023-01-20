#
#  Target.rb
#  Goldsmith
#
#  Created by Tim Walters on 4/25/08.
#  Copyright 2014 by Avid Technology, Inc.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool

=begin rdoc
  Represents a target machine and its gst process. All communication to the system and application
  under test ultimately goes through here.
=end
  class Target

    DEFAULT_TIMEOUT = 1200.0
    SHORT_TIMEOUT = 10.0

    attr_reader :addr, :sys_info, :platform, :pt_type, :selected_volumes

    def initialize(addr = 'localhost', timeout = DEFAULT_TIMEOUT)
      @addr = addr
      @timeout = timeout
      @sys_info = nil
      @gst = local? ? LocalTarget.new : RemoteTarget.new(addr, timeout)
      connect_to_target(@timeout)
      @selected_volumes = []
      @platform = get_platform
      @debug_log_path = nil
      set_timeout_factor if available?
    end

    def self.local
      return self.new('localhost')
    end
    
=begin rdoc
  Notifies target that a test is in progress. Do not call manually.
=end
    def start_testing
      return do_transaction(:start_testing, {}, nil)
    end

=begin rdoc
  Notifies target that test is complete. Do not call manually.
=end
    def stop_testing
      return do_transaction(:stop_testing, {}, nil)
    end

=begin rdoc
  Gets the contents of a file from target. Decrypt argument decodes a digitrace file.
=end
    def get_file(path, timeout = nil, decrypt = false)
      if decrypt
        return do_transaction(:get_file, path, timeout).decrypt
      else
        return do_transaction(:get_file, path, timeout)
      end
    end

    def get_sources_folder
      return do_transaction(:get_sources_folder, {}, nil)
    end

    def build_folder
      return do_transaction(:build_folder, {}, nil)
    end

    def build_folder=(path)
      return do_transaction(:build_folder=, path, nil)
    end

    def real_path(path)
      real_path = perform_on_target(Pathname.new(path), :realpath).to_s
      real_path.gsub!('/', '\\') if win?

      return real_path
    end

=begin rdoc
  Deletes the file from the target.
=end
    def delete_file(path, timeout = nil)
      return do_transaction(:delete_file, path, timeout)
    end

=begin rdoc
Deletes Pro Tools preferences and databases on target machine.
=end
    def delete_pt_prefs(timeout = nil)
      return do_transaction(:delete_pt_prefs, {}, timeout)
    end

=begin rdoc
Gets target platform.
=end
    def get_platform(timeout = nil)
      return do_transaction(:get_platform, {}, timeout)
    end

=begin rdoc
Kills dsh on target machine
=end
    def kill_dsh(timeout = 60)
      TEST_INFO.add_kill
      result = do_transaction(:kill_dsh, {:timeout => timeout}, timeout + 10)
      (result ? "dsh killed." : "Couldn't kill dsh!").warn
    end

=begin rdoc
Launches custom application on target machine.
=end
    def launch_app(app_class, app_name, app_options = [])
      result = do_transaction(:launch_app, { :app_class => app_class, :app_name => app_name, :app_options => app_options, :local_target => local?}, DEFAULT_TIMEOUT)
      if result.kind_of?(Exception)
        "Failed to launch app: #{result}.".abort(self)
      else
        "launch_app #{result}".log_verbose
      end

      return app_class.new(self)
    end

=begin rdoc
Returns true if dsh is running on target machine.
=end
    def dsh_running?(timeout = nil)
      return do_transaction(:dsh_running?, {}, timeout, false)
    end

=begin rdoc
Loads system and volume info from target machine.
=end
    def load_info(timeout = nil)
      info = do_transaction(:get_sys_info, {}, timeout)
      if not info.nil?
        @sys_info = info
      end
      return self
    end
	
=begin rdoc
Pings target machine. Returns true or false.
=end
    def ping(timeout = nil)
      return do_transaction(:ping, {}, timeout) === :gnip
    end

=begin rdoc
Sets base timeout for all queries. Will be multiplied by
global timeout_factor.
=end
    def set_timeout(timeout)
      @timeout = timeout
    end

=begin rdoc
Don't use directly, prefer App over it.
=end
    def send_command_to_app(app_cmd)
      return do_transaction(:send_command_to_app, app_cmd, DEFAULT_TIMEOUT)
    end

=begin rdoc
Don't use directly, prefer App over it.
=end
    def close_app
      return do_transaction(:close_app, nil, SHORT_TIMEOUT)
    end

=begin rdoc
Sending commands always checks for alert, so we only need to call this directly right after PT launch.
=end
    def check_for_alert(options = {})
      return do_transaction(:check_for_alert, options, options[:timeout], false)
    end

=begin rdoc
  Returns true if target is local.
=end
    def local?
      return ['localhost'].include?(@addr)
    end

=begin rdoc
  Returns true if target is available for testing
=end
    def available?(timeout = nil)
      return do_transaction(:available?, {}, timeout)
    end

=begin rdoc
  Returns true if target is busy.
=end
    def busy?(timeout = nil)
      return false
    end

    def move_pt_options(options = {})
      return do_transaction(:move_pt_options, options, options[:timeout])
    end

    def restore_pt_options(options = {})
      return do_transaction(:restore_pt_options, options, options[:timeout])
    end

=begin rdoc
    Calls obj.method(args) { block } on the target, allowing us to perform
    arbitrary Ruby operations there. Be careful!
=end
    def perform_on_target(obj, method, args = nil, block = nil, options = {})
      return do_transaction(
        :perform_on_target,
        options.update(
          :obj => obj,
          :method => method,
          :args => args,
          :block => block
        ),
        options[:timeout]
      )
    end

=begin rdoc
  Returns Pathname to root resource folder on target machine.
=end 
    def resource_folder
      rf_video = self.video_volumes.first.path + RESOURCE_FOLDER unless self.video_volumes.first.nil?
      rf_boot = self.boot_volume.path + RESOURCE_FOLDER

      if !defined?(rf_video) && !rf_video.nil? && exist_on_target?(rf_video)
        rf = rf_video
      elsif !rf_boot.nil? && exist_on_target?(rf_boot)
        rf = rf_boot
      else
        "Resource folder not installed on target.".abort(self)
      end

      return rf
    end

=begin rdoc
  Returns path (on target machine) to a test resource. If name is nil,
  returns appropriate resource folder. Returns a Pathname.
=end
    def resource_path(resource_type = TestResourceType.audio, name = nil)
      folder_path = resource_folder + TestResourceType.new(resource_type).to_s
      path = name.nil? ? folder_path : folder_path + name
      "Resource #{path} not installed on target.".abort(self) if !exist_on_target?(path)
      return path
    end

=begin rdoc
  Return true if path exists on target, false otherwise.
=end  
    def exist_on_target?(path, options = {})
      return do_transaction(:exist_on_target?, path, options[:timeout])
    end

=begin rdoc
Selects a menu item in main Pro Tools menus (including submenus, but not including pop-ups).
+menu_path+ is a Posix-style path to the menu item, must include the root slash, and must match the menu 
item text exactly. Example: <tt>"/File/New Session..."</tt>

=begin rdoc
Returns +true+ if target is a Mac, false otherwise.
=end
    def mac?
      return (@platform == :mac)
    end

=begin rdoc
Returns +true+ if target is a Windows machine, false otherwise.
=end
    def win?
      return (@platform == :win)
    end

# :stopdoc:
=begin rdoc
Kill gst process on target
=end
    def quit(options = {})
      return do_transaction(:quit, options, options[:timeout])
    end

    def get_current_arg_value(arg_class, options = {})
      "Can't get current value for #{arg_class.name}.".abort if !arg_class.respond_to?(:current)
      return arg_class.current(self, options)
    end

    # :startdoc:

    private

    def set_timeout_factor
      return do_transaction(:set_timeout_factor, TEST_INFO.timeout_factor, nil)
    end

    def connect_to_target(timeout = DEFAULT_TIMEOUT)
      done = false
      last_err = nil
      succeeded = false
      begin
        Timeout::timeout(timeout + 1) do
          while !done
            begin
              ping(timeout)
              succeeded = true
              done = true
            rescue Exception => err
              last_err = err
              sleep(2.0)
            end
          end
        end
      rescue TimeoutError
        succeeded = false
      end
      "Error (#{last_err.class.name}) connecting to target #@addr: #{last_err}.".abort if !succeeded
    end

    def do_transaction(id, data, timeout, trace = true, connect_timeout = DEFAULT_TIMEOUT)
      return @gst.do_transaction(id, data, timeout, trace = true, connect_timeout = DEFAULT_TIMEOUT)
    end

=begin rdoc
We use this Ruby feature to allow queries to Target's SysInfo object to appear to be
methods of Target to the user.
=end
    def method_missing(symbol, *args)
      if SysInfo.method_defined?(symbol) then
        if @sys_info.nil? then
          "Can't call #{symbol} on a #{self.class} before loading system info.".abort(self)
        else
          return @sys_info.send(symbol)
        end
      else
        raise(NoMethodError, "undefined method \'#{symbol}\' for #{self.class}")
      end
    end
  end # Target

  class LocalTarget

    def initialize(build_folder = nil, verbose = false)
      @local_gst = DTTMonitor.new(build_folder, verbose)
    end

    def do_transaction(id, data, timeout, trace = true, connect_timeout = Target::DEFAULT_TIMEOUT)
      cmd = TargetCmd.new(id, data)
      if trace && LOG_MANAGER.logging?
        data_string = data.respond_to?(:log_string) ? data.log_string : data.inspect
        "Send: #{id.to_s}, #{data_string}".log_verbose
      end
      result = nil
      raw_timeout = timeout.or_if_nil(Target::DEFAULT_TIMEOUT)
      timeout_amt = (raw_timeout * TEST_INFO.timeout_factor) + 10.0
      Timeout::timeout(timeout_amt) do
        result =  @local_gst.send(cmd.id, cmd.data)
      end #Timeout::timeout
      raise result if result.kind_of?(Exception)
      if trace && LOG_MANAGER.logging?
        result_string = result.respond_to?(:log_string) ? result.log_string : result.inspect
        result_string = "FILE RECEIVED" if (/get_file/.match(id.to_s))
        "Recv: #{result_string}".log_verbose
      end
      return result
    end
  end

  class RemoteTarget
    def initialize(addr, timeout)
      @addr = addr
      @timeout = timeout
    end

    def do_transaction(id, data, timeout, trace = true, connect_timeout = Target::DEFAULT_TIMEOUT)
      cmd = TargetCmd.new(id, data)
      if trace && LOG_MANAGER.logging?
        data_string = data.respond_to?(:log_string) ? data.log_string : data.inspect
        "Send: #{id.to_s}, #{data_string}".log_verbose
      end
      result = tc = nil
      interrupted = false
      # For these timeouts, we use GSTargetTimeoutError. If we use the default TimeoutError,
      # then any timeouts that wrap a target communication may get rescued here instead of
      # in the outer scope.
      begin
        Timeout::timeout(connect_timeout, DTTTargetTimeoutError) do
          tc = TargetConnection.new(@addr)
        end
      rescue SystemCallError, IOError, SocketError => err
        tc.close if !tc.nil?
        attrib = err.class.name
        "Error (#{attrib}) connecting to target machine: #{err}.".abort(self)
      rescue DTTTargetTimeoutError => err
        raise(DTTTargetTimeoutError, "Error (#{attrib} [#{connect_timeout}]) connecting to target machine: #{err}.")
      end
      begin
        raw_timeout = connect_timeout.or_if_nil(@timeout).max(timeout.or_if_nil(Target::DEFAULT_TIMEOUT))
        timeout_amt = (raw_timeout * TEST_INFO.timeout_factor) + 10.0
        Timeout::timeout(timeout_amt, DTTTargetTimeoutError) do
          |timeout_length|
          cmd.timeout = timeout_length
          tc.print(Marshal.dump(cmd))
          result = Marshal.load(tc)
        end
      rescue Interrupt => err
        interrupted = true
      rescue SocketError, SystemCallError, IOError => err
        "Error (#{err.class.name}) communicating with target machine (#{cmd}): #{err}.".abort(self)
      rescue DTTTargetTimeoutError => err
        raise(DTTTargetTimeoutError, "Error (#{err.class.name} [#{timeout_amt}]) communicating with target machine: #{err}.")
      end
      raise err if interrupted
      "Target machine is busy.".abort(self) if result == :busy_signal
      raise result if result.kind_of?(Exception)
      if trace && LOG_MANAGER.logging?
        result_string = result.respond_to?(:log_string) ? result.log_string : result.inspect
        result_string = "FILE RECEIVED" if (/get_file/.match(id.to_s))
        "Recv: #{result_string}".log_verbose
      end
      return result
    end
  end

  ##### TargetCmd

  class TargetCmd   # :nodoc:

    def initialize(id, data = {})
      @id = id
      @data = data
      @sender = RUNTIME_INFO.unique_id
    end

    def to_s
      return "#@id: #{@data.inspect}"
    end

    attr_reader :id, :data, :sender
    attr_accessor :timeout

  end # TargetCmd

  ##### TargetConnection

  class TargetConnection < TCPSocket    # :nodoc:

    def initialize(host)
      return super(host, MONITOR_PORT)
    end

  end # TargetConnection

end # module
