#
#  RDSI.rb
#
#  Created by Dmytro Pravdiuk in October 2021.
#  Copyright 2021 by Avid Technology, Inc.
#

module DishTestTool

=begin rdoc
Class for storing and reading device attributes
=end
  class DeviceInfo
    def initialize(id, info)
      @id = id
      @name = info["name"]
      @uid = info["uid"]
      @card_index = info["card_index"]
      @card_type = info["card_type"]
      @fw_ver = info["fw_ver"]
      @is_available = info["is_available"]
      @is_fw_ok = info["is_fw_ok"]
      @is_hw_ok = info["is_hw_ok"]
    end

    def id
      return @id
    end

    def name
      return @name
    end

    def uid
      return @uid
    end

    def card_index
      return @card_index
    end

    def card_type
      return @card_type
    end

    def fw_ver
      return @fw_ver
    end

    def is_available
      return @is_available
    end

    def is_fw_ok
      return @is_fw_ok
    end

    def is_hw_ok
      return @is_hw_ok
    end

  end

=begin rdoc
Class for interacting with RDSI dish
=end

  class RDSI

    WAIT_TIME_FOR_DNSSD = 1
    DEVICE_PROPERTY_Fullinfo = "full_info"
    DEVICE_PROPERTY_Name = "name"
    DEVICE_PROPERTY_Uid = "uid"
    DEVICE_PROPERTY_Fw = "fw"
    DEVICE_PROPERTY_NetworkInterface = "network_iface"
    DEVICE_PROPERTY_Ipv4 = "ipv4"
    DEVICE_PROPERTY_Ipv6 = "ipv6"
    DEVICE_PROPERTY_HostIpv4 = "host_ipv4"
    DEVICE_PROPERTY_HostIpv6 = "host_ipv6"
    
    def initialize(dsh)
      print("Loading RDSI dish... ")
      @dsh = dsh
      @dsh.load_dish(["RDSI"])
      puts "done"
    end

    def get_device_infos(max_tries=30)
      sleep WAIT_TIME_FOR_DNSSD # wait for dns discovery completion

      pass = false
      tries = 0
      all_devices = {}

      until pass || tries == max_tries do
        tries += 1
        next if @dsh.get_device_infos == ""
        begin
          @dsh.get_device_infos.each do |id, info|
            unless all_devices.key?("#{id}")
              device_info = DeviceInfo.new(id, info)
              all_devices.merge!("#{id}" => device_info)
            end
          end
        rescue Exception => res
          p res
          return :fail
        end
        pass = !(all_devices.empty?)
      end
      print "."
      return all_devices
    end

    def get_the_first_zeta_device_info
      all_devices = get_device_infos
      id, device_info = all_devices.select{|id, dev| dev.card_type.start_with?("Zeta")}.first
      return device_info

    end

    def get_device_info_by_uid(uid)
      all_devices = get_device_infos
      id, device_info = all_devices.select{|id, dev| dev.card_type.start_with?("Zeta") && dev.uid == uid}.first
      return device_info

    end

    def get_device_property(dsi_device_index, property)
      return @dsh.get_device_property({'device_index' => dsi_device_index, 'property' => property})
    end

    def get_dsps
      return @dsh.get_dsps
    end

    def get_number_of_dsps
      return @dsh.get_number_of_dsps
    end

    def init_device(id)
      res = @dsh.init_device(id)
      raise("Can't init device") if res == :fail
      return res
    end

    def uninit_device(id)
      res = @dsh.uninit_device(id)
      raise("Can't deinit device") if res == :fail
      return res
    end

    #   Utils
    def get_ipv4 (dsi_device_index)
      res = @dsh.get_device_property({'device_index' => dsi_device_index, 'property' => "ipv4"})
      raise("Can't init device") if res == :fail
      return res
    end

    def get_ipv6 (dsi_device_index)
      res = @dsh.get_device_property({'device_index' => dsi_device_index, 'property' => "ipv6"})
      raise("Can't init device") if res == :fail
      return res
    end

    def get_zeta_info(device_type="Zeta8", max_tries=30)

      pass = false
      tries = 0
      print "Gathering info about Zeta devices..."
      sleep WAIT_TIME_FOR_DNSSD # wait for dns discovery completion

      until pass || tries == max_tries do

        begin
          all_devices = @dsh.get_device_infos
          tries += 1
          next if all_devices == ""

          id, info = zeta_devices = all_devices.select{|key, hash| hash["card_type"].start_with?(device_type)}.first

        rescue Exception => res
          p res
          raise("Error finding any Zeta device.")
        end

        raise("Can't get device index.") if id == :fail
        pass = !(id.to_s.empty?)

      end

      print "."
      puts " done"

      raise("Can't find any Zeta device.") if !pass
      return id, info

    end

    def wait4device(uid, wait_for_reboot=60)
      
      wait_time = 0
      available = :false
      print(" Waiting for device to reboot...")
      sleep(5)

      # wait for device to appear in get_device_infos
      loop do
        begin
          all_devices = @dsh.get_device_infos
          found = all_devices.any?{|key, hash| hash["uid"] == uid}
        rescue Exception => e
          print "."
          wait_time +=3
          sleep(3)
        else
          break
        end

        raise "Reboot timeout error. Waiting > #{wait_for_reboot} seconds." if wait_time >= wait_for_reboot
      end

      # wait for device to have is_available = true
      loop do
        all_devices = @dsh.get_device_infos
        all_devices.each do |device|
          available = :true if device[1]["uid"] == uid && device[1]["is_available"]
          @dsh.refresh([device[0].to_i])
        end

        if available == :true
          puts " done"
          sleep(3)
          break
        end

        print "."
        wait_time +=3
        sleep(3)

        raise "Availability timeout error. Waiting > #{wait_for_reboot} seconds." if wait_time >= wait_for_reboot
      end

    end

  end

end # module
