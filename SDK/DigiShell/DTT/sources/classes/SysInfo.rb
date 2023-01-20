#
#  SysInfo.rb
#  Goldsmith
#
#  Created by Tim Walters on 4/25/08.
#  Copyright 2014 by Avid Technology, Inc.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool

  require 'KernelAdditions'

  # =begin rdoc
  #  Holds system-specific information. Called on launch by both master and target.
  #  =end
  class SysInfo

    UNKNOWN_VALUE = 'Unknown'
    ONE_MEG = 1024 * 1024
    ONE_GIG = ONE_MEG * 1024

    # This defines order for tab-delimited and pretty-printed output
    @@display_keys = [:computer_name, :ip_addr, :os, :os_version, :memory, :cpu_type, :num_procs, :cpu_speed, :volumes]

    attr_reader :computer_name, :ip_addr, :memory, :cpu_type, :boot_id, :num_procs, :cpu_speed, :volumes

    # Volume Info Keys
    @@volume_info_keys = [:drive_type, :file_system, :free_space, :media_type, :capacity]

    @@windows_volume_types = {
      0 => 'Unknown',
      1 => 'No Root Dir',
      2 => 'Removable',
      3 => 'Fixed',
      4 => 'Remote',
      5 => 'CD-ROM',
      6 => 'RAM Disk'
    }

    def initialize
      load
    end

    public

    # kludge to avoid Ruby marshalling bug
    def boot_volume
      return @volumes.find { |v| v.id == @boot_id }
    end

    def current_volume
      pwd = Dir.getwd
      if mac?
        return /^\/Volumes\//.match(pwd) ? @volumes.find { |v| v.name == pwd.split('/')[2] } : boot_volume
      else
        return @volumes.find { |v| /^#{v.id}/.match(pwd) }
      end
    end

    def SysInfo.get_ip
      begin
        orig, Socket.do_not_reverse_lookup = Socket.do_not_reverse_lookup, true  # turn off reverse DNS resolution temporarily
        UDPSocket.open do |s|
          s.connect '64.233.187.99', 1
          s.addr.last
        end
      rescue
        return "127.0.0.1"
      end
    ensure
      Socket.do_not_reverse_lookup = orig
    end

    def os
      return @rt_info.os
    end

    def os_version
      return @rt_info.os_version
    end

    def linux?
      return @rt_info.linux?
    end

    def mac?
      return @rt_info.mac?
    end

    def win?
      return @rt_info.win?
    end

    def xp?
      return @rt_info.xp?
    end

    def win_seven?
      return @rt_info.win_seven?
    end

    def vista?
      return @rt_info.vista?
    end

    # :stopdoc:

    def self.display_keys
      return @@display_keys
    end

    def log_string
      return @@display_keys.collect {
        |key|
        item = self.send(key)
        item_string = item.respond_to?(:collect) ? item.collect { |x| x.to_s }.join(', ') : item.to_s
        key.to_s.upcase + ': ' + item_string
      }.join("\n")
    end

    # :startdoc:

    private

    def load
      begin
        load_body
      rescue SystemCallError, IOError => err
        "#{self.class} could not be initialized: #{err}.".abort(self)
      end
    end

    # TO_DO: clean up and use current style
    def load_body

      @ip_addr = SysInfo.get_ip
      @volumes = []
      config = (RUBY_VERSION < "1.9.3") ? "Config" : "RbConfig"
      if /apple/.match(eval("#{config}::CONFIG['host']"))
        @platform = :mac
      elsif /linux/.match(eval("#{config}::CONFIG['host']"))
        @platform = :linux
      else
        @platform = :win
      end
      @rt_info = RuntimeInfo.new
      if @rt_info.win?

        # WINDOWS
        ole = WIN32OLE.connect("winmgmts:\\\\.")
        oses = ole.InstancesOf('Win32_OperatingSystem')
        oses.each {
          |os|
          @memory = sprintf("%.2f GB", os.TotalVisibleMemorysize.to_f / ONE_MEG)
        }
        cses = ole.InstancesOf('Win32_ComputerSystem')
        cses.each {
          |cs|
          @num_procs = ENV['NUMBER_OF_PROCESSORS']
        }
        procs = ole.InstancesOf('Win32_Processor')
        procs.each {
          |proc|
          @cpu_type = proc.Caption
          @cpu_speed = sprintf("%.2f GHz", proc.MaxClockSpeed.to_f / 1000.0)
        }
        @boot_id = nil
        volumes = ole.InstancesOf('Win32_LogicalDisk')
        volumes.each {
          |volume|
          volName = volume.VolumeName
          # Not sure why volume.Access returns nil, but it does, so we exclude non-fixed rather
          # than read-only, which will at least catch CDs and such
          if (@@windows_volume_types[volume.DriveType] == 'Fixed') && !volName.nil? then
            vol_obj = Volume.new(
            volName,
            volume.Name + '/', # Ruby doesn't add this when joining paths, which chokes on Vista
            sprintf("%.2f GB", volume.Size.to_f / ONE_GIG),  # capacity
            sprintf("%.2f GB", volume.FreeSpace.to_f / ONE_GIG), # free space
            @@windows_volume_types[volume.DriveType].or_if_nil('Unknown'),  # drive type
            volume.FileSystem,
            @platform
            )
            @volumes.push(vol_obj)
            if /^#{ENV['SystemDrive']}/i.match(vol_obj.id)
              @boot_id = vol_obj.id
              @computer_name = volume.SystemName
            end
          end
        }
      elsif @rt_info.linux?

      else
        hardware_overview = Hash.new
        disks_overview = Array.new

        `system_profiler SPHardwareDataType`.split(/\n+/).each  do |line|
          reg_exp_result = line.match(/^\s*(.*):\s*(.*)/)
          hardware_overview[reg_exp_result[1]] = reg_exp_result[2] if !reg_exp_result.nil?
        end

        Pathname.new("/Volumes").children.each do |volume|

          if system("diskutil info '#{volume.to_s}'", :out => File::NULL, :err => File::NULL)
            
            diskutil_info = {}
            `diskutil info \"#{volume.to_s}\"`.each_line do |line|
              regexp_result = /^\s*(.*):\s*(.*)/.match(line)
              diskutil_info[regexp_result[1]] = regexp_result[2] if !regexp_result.nil?
              diskutil_info[:platform] = :mac
            end
            disks_overview.push(diskutil_info)
          
            end
        end

        #some cleanup
        disks_overview.reject!{|disk| disk.has_key?("Could not find disk") or disk["Read-Only Volume"].eql?("Yes")}

        disks_overview.map do |disk|
          disk['Mount Point'] += "Volumes/#{disk['Volume Name']}" if disk['Mount Point'].eql?('/')
          disk['Container Free Space'] ||= disk['Container Available Space']
          disk['Container Free Space'] = disk['Container Free Space'][/^(\d*\.?\d*\s\w+)/]
          disk['Total Size'] ||= disk['Container Total Space']
          disk['Total Size'] = disk['Total Size'][/^(\d*\.?\d*\s\w+)/]
        end

        @memory = hardware_overview['Memory']
        @computer_name = `hostname -s`.chomp

        @cpu_type = hardware_overview['Processor Name']
        @cpu_speed = hardware_overview['Processor Speed']
        @num_procs = hardware_overview['Number of Processors']

        @volumes = []
        disks_overview.each { |d|
          @volumes.push(Volume.new(d['Volume Name'], d['Mount Point'], d['Volume Free Space'], d['Total Size'], d['Protocol'], d['Type (Bundle)'], d[:platform]))
        }

      end
      return self
    end
  end # Sys_Info

end # module
