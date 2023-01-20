#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

module DishTestTool

=begin rdoc
  Singleton class for holding runtime environment info. At startup, instantiated
  in global variable RUNTIME_INFO. Both master and target have one, so don't mix 'em up.
=end
  class RuntimeInfo

    attr_reader :platform, :os, :os_version, :username, :ip_addr, :unique_id, :home_dir, :dsh_log_files, :debug_build
    attr_writer :dsh_log_files, :debug_build

    def initialize
      config = (RUBY_VERSION < "1.9.3") ? "Config" : "RbConfig"
      if /apple/.match(eval("#{config}::CONFIG['host']"))
        @platform = :mac
      elsif /linux/.match(eval("#{config}::CONFIG['host']"))
        @platform = :linux
      else
        @platform = :win
      end
      get_os
      @username = ENV['USERNAME'].or_if_nil(ENV['LOGNAME'])
      @ip_addr = SysInfo.get_ip
      @unique_id = "#{@ip_addr}.#{Process.uid}"
      if mac? or linux?
        @home_dir = Pathname.new(ENV['HOME'])
      else
        # HOMEPATH is a relative path, but stored in the form of an
        # absolute path, so we move to the current root and then
        # expand
        cwd = Pathname.getwd
        if ENV['HOMEDRIVE']
          Dir.chdir(ENV['HOMEDRIVE'])
          @home_dir = Pathname.new(ENV['HOMEPATH']).expand_path
        elsif ENV['USERPROFILE']
          @home_dir = Pathname.new(ENV['USERPROFILE']).expand_path
        else
          raise "Couldn't find HOMEDRIVE or USERPROFILE in your environment variables."
        end
        Dir.chdir(cwd)
      end
      
      @dsh_log_files = Array.new
      
    end

=begin rdoc
  Returns true if running on a Mac.
=end
    def mac?
      return @platform == :mac
    end

=begin rdoc
  Returns true if running on a Linux.
=end
    def linux?
      return @platform == :linux
    end

=begin rdoc
  Returns true if running on some flavor of Windows.
=end
    def win?
      return @platform == :win
    end

=begin rdoc
  Returns true if running on Windows Vista.
=end
    def vista?
      return win? && !/VISTA/.match(@os.upcase).nil?
    end

=begin rdoc
  Returns true if running on Windows 7, false otherwise.
=end
    def win_seven?
      return win? && !/WINDOWS 7/.match(@os.upcase).nil?
    end

=begin rdoc
  Returns true if running on Windows XP, false otherwise.
=end
    def xp?
      return win? && !vista? && !win_seven?
    end

=begin rdoc
  Returns a string describing the current OS and its version.
=end
    def get_os
      if win?
        ole = WIN32OLE.connect("winmgmts:\\\\.")
        oses = ole.InstancesOf('Win32_OperatingSystem')
        oses.each {
            |os|
          @os = os.Caption
          @os_version = os.Version
        }
      elsif linux?
        @os = "Linux"
		if system("which lsb_release > /dev/null")
        	@os_version = `lsb_release -d`.split(' ')[1]
		else
			@os_version = 'Unknown'
		end
      else
        @os = "Mac OS X"
        os_info = {}
        `sw_vers`.split(/\n+/).each  do |line|
          reg_exp_result = line.match(/^\s*(.*):\s*(.*)/)
          os_info[reg_exp_result[1]] = reg_exp_result[2] if !reg_exp_result.nil?
        end
        @os_version = "#{os_info['ProductVersion']} (Build #{os_info['BuildVersion']})"
      end
    end
    
=begin rdoc
  Returns True for Debug build, False for Release.
=end
    def debug_build?
      return self.debug_build
    end

=begin rdoc
  Returns currently available Goldsmith scripts as a Set of classes.
=end
    def known_scripts
      ks = Set.new
      ObjectSpace.each_object(Class) do
      |c|
        if c.respond_to?(:is_a_dtt_script_class?)
          ks.add(c)
        end
      end
      return ks
    end

=begin rdoc
  Returns the script class with the name +script_name+, or nil.
=end
    def get_script_class_by_name(script_name)
      return known_scripts.find { |s| s.short_name == script_name}
    end

    def dev_dtt_dir
      root_tree = SOURCE_DIR.parent.parent.parent.parent
      dev_dir = root_tree + (mac? ? Pathname.new('MacBag/Debug') : Pathname.new('WinBag/Debug/bin'))
      return dev_dir
    end

  end

end
