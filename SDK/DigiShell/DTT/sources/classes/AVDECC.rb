#
#  AVDECC.rb
#
#  Created by Sergii Anisienko in January 2022
#  Copyright 2022 by Avid Technology, Inc.
#

=begin rdoc
Class for interacting with AVDECC dish
=end

module DishTestTool
  class AVDECC
    ASYNC_DELAY = 1
    def initialize(dsh)
      print("Loading AVDECC dish... ")
      @dsh = dsh
      @dsh.load_dish(["AViDECC"])
      puts "done"
    end
    
    def version
      
      res = @dsh.version
      
    end
    
    def discover_expanded_controller
      
      res = @dsh.cmd('discover-expanded-controller')
      sleep(ASYNC_DELAY)
      
      return res
      
    end

    def list_controllers

      res = @dsh.cmd('list-controllers')

    end

    def list_available_entities

      res = ""
      10.times do
        res = @dsh.cmd('list-available-entities')
        break if !res.empty?
        sleep(ASYNC_DELAY)
      end

      raise('No available entities are present.') if res.empty?
      return res

    end

    def list_network_interfaces

      res = @dsh.cmd('list-network-interfaces')
      raise('No available network interfaces are present.') if res.empty?

      return res

    end

    def list_audio_units(entity_uid, configuration_index)

      res = @dsh.cmd('list-audio-units', [entity_uid, configuration_index])

    end

    def list_configurations(entity_uid)

      res = @dsh.cmd('list-configurations', entity_uid)

    end

    def list_clock_sources(entity_uid, configuration_index)

      res = @dsh.cmd('list-clock-sources', [entity_uid, configuration_index])

    end

    def list_clock_domains(entity_uid, configuration_index)

      res = @dsh.cmd('list-clock-domains', [entity_uid, configuration_index])

    end

    def list_controls(entity_uid, configuration_index)

      res = @dsh.cmd('list-controls',[entity_uid, configuration_index])

    end

    def list_memory_objects(entity_uid, configuration_index)

      res = @dsh.cmd('list-memory-objects',[entity_uid, configuration_index])

    end

    def list_input_streams(entity_uid, configuration_index)

      res = @dsh.cmd('list-input-streams', [entity_uid, configuration_index])

    end

    def list_output_streams(entity_uid, configuration_index)

      res = @dsh.cmd('list-output-streams', [entity_uid, configuration_index])

    end

    def list_groups(controller)

      res = @dsh.cmd('list-groups', controller)

    end
    
    def create_controller(iface, name)
    
      res = @dsh.cmd('create-controller', [iface, name])
      sleep(ASYNC_DELAY)
    
    end

    def create_expanded_controller(iface)

      res = @dsh.cmd('create-expanded-controller', iface)
      sleep(ASYNC_DELAY)

    end

    def destroy_controller(names)

      res = @dsh.cmd('destroy-controller', names)
      sleep(ASYNC_DELAY)

    end

    def connect(entity_uid_src, stream_index_src, entity_uid_dst, stream_index_dst)

      res = @dsh.connect([entity_uid_src, stream_index_src, entity_uid_dst, stream_index_dst])

    end

    def disconnect(entity_uid_src, stream_index_src, entity_uid_dst, stream_index_dst)

      res = @dsh.disconnect([entity_uid_src, stream_index_src, entity_uid_dst, stream_index_dst])

    end

    def acquire_entities(entity_uids)

      res = @dsh.cmd('acquire-entities', entity_uids)

    end

    def release_entities(entity_uids)

      res = @dsh.cmd('release-entities', entity_uids)

    end

    def set_audio_unit_sampling_rate(entity_uid, audio_unit_index, desired_rate)

      res = @dsh.cmd('set-audio-unit-sampling-rate', [entity_uid, audio_unit_index, desired_rate])

    end

    def set_entity_configuration(entity_uid, configuration_index)

      res = @dsh.cmd('set-entity-configuration', [entity_uid, configuration_index])

    end

    def set_input_stream_format(entity_uid, configuration_index, stream_index, desired_format)

      res = @dsh.cmd('set-input-stream-format', [entity_uid, configuration_index, stream_index, desired_format])

    end

    def set_output_stream_format(entity_uid, configuration_index, stream_index, desired_format)

      res = @dsh.cmd('set-output-stream-format', [entity_uid, configuration_index, stream_index, desired_format])

    end

    def start_input_streams(entity_uid, stream_indices)

      res = @dsh.cmd('start-input-streams', [entity_uid, stream_indices])

    end

    def start_output_streams(entity_uid, stream_indices)

      res = @dsh.cmd('start-output-streams', [entity_uid, stream_indices])

    end

    def stop_input_streams(entity_uid, stream_indices)

      res = @dsh.cmd('stop-input-streams', [entity_uid, stream_indices])

    end

    def stop_output_streams(entity_uid, stream_indices)

      res = @dsh.cmd('stop-output-streams', [entity_uid, stream_indices])

    end

    def set_expanded_slot(slot_index, entity_uid)

      res = @dsh.cmd('set-expanded-slot', [slot_index, entity_uid])

    end

    def set_expanded_mode(mode)

      res = @dsh.cmd('set-expanded-mode', mode)
      sleep(ASYNC_DELAY)
      
      return res

    end

    def remove_expanded_slot(slot_index)

      res = @dsh.cmd('remove-expanded-slot', slot_index)

    end

    def list_expanded_slots

      res = @dsh.cmd('list-expanded-slots')

    end

    def list_expanded_configuration

      res = @dsh.cmd('list-expanded-configuration')

    end

    def get_expanded_config(configuration_uid)

      res = @dsh.cmd('get-expanded-config', configuration_uid)

    end
    
    def set_expanded_configuration(configuration_uid)
      
      res = @dsh.cmd('set-expanded-configuration', configuration_uid)
      
    end

    def create_custom_entity(base_name, interface_name, xml_path)

      res = @dsh.cmd('create-custom-entity', [base_name, interface_name, xml_path])

    end

    def cleanup_custom_entities

      res = @dsh.cmd('cleanup-custom-entities')

    end

    def get_entity_avbstatus(entity_uid)

      res = @dsh.cmd('get-entity-avbstatus', entity_uid)

    end

    def get_mac_grandmasterid(controller)

      res = @dsh.cmd('get-mac-grandmasterid', controller)

    end

    def register_callback(callback_name, entity_uid) #DeviceInfo ExpandedSlot ExpandedConfiguration AvbStatus

      res = @dsh.cmd('register-callback', [callback_name, entity_uid])

    end

    def unregister_callback(callback_uid)

      res = @dsh.cmd('unregister-callback', callback_uid)

    end

    def request_clocking_source(controller, group_id, desired_source)

      res = @dsh.cmd('request-clocking-source', [controller, group_id, desired_source])

    end

    def request_sampling_rate(controller, group_id, desired_rate)

      res = @dsh.cmd('request-sampling-rate', [controller, group_id, desired_rate])

    end

  end # class

=begin rdoc
Class for interacting with AVDECC entity config
=end

  class AVBConfig

    attr_reader :idx, :name, :audio_units, :clock_domains, :clock_sources, :controls, :memory_objects, :input_streams, :output_streams
    attr_writer :idx, :name, :audio_units, :clock_domains, :clock_sources, :controls, :memory_objects, :input_streams, :output_streams
    def initialize(avdecc, uid, config)

      @avdecc = avdecc
      @uid = uid
      self.idx = config['id']
      self.name = config['name']
      print("Collecting current AVBConfig info... ")

      self.update

      print "done "

    end

    def update

      self.audio_units = @avdecc.list_audio_units(@uid, self.idx)
      self.clock_domains = @avdecc.list_clock_domains(@uid, self.idx)
      self.clock_sources = @avdecc.list_clock_sources(@uid, self.idx)
      self.controls = @avdecc.list_controls(@uid, self.idx)
      self.memory_objects = @avdecc.list_memory_objects(@uid, self.idx)
      self.input_streams = @avdecc.list_input_streams(@uid, self.idx)
      self.output_streams = @avdecc.list_output_streams(@uid, self.idx)

    end

    def info

      print "Config idx: "
      puts self.idx
      print "Config name: "
      puts self.name
      puts "Audio Units: "
      puts self.audio_units
      puts "Clock Domains: "
      puts self.clock_domains
      puts "Clock Sources: "
      puts self.clock_sources
      puts "Controls: "
      puts self.controls
      puts "Memory Objects: "
      puts self.memory_objects
      puts "Input Streams: "
      puts self.input_streams
      puts "Output Streams: "
      puts self.output_streams
      puts "<<<"

    end

  end # class

=begin rdoc
Class for interacting with AVDECC entity
=end

  class AVBEntity

    attr_reader :controller, :uid, :name, :fw, :configs, :config_id, :current_config, :mac_gmid, :avbstatus, :type, :available
    attr_writer :controller, :uid, :name, :fw, :configs, :config_id, :current_config, :mac_gmid, :avbstatus, :type, :available
    def initialize(avdecc, controller, uid)

      @avdecc = avdecc
      self.controller = controller
      self.uid = uid

      print("Collecting AVBEntity info... ")

      self.update

      puts "done"

    end

    def update

      self.available = false

      entity_info = @avdecc.list_available_entities.detect {|entity| (entity['id'] == self.uid) }
        
      self.name = entity_info['entity-name']
      TEST_INFO.add_warning('Empty entity <name>: ' + self.uid.to_s(16)) if self.name.to_s.empty?
      self.fw = entity_info['firmware-version']
      TEST_INFO.add_warning('Empty entity <firmware-version>: ' + self.uid.to_s(16)) if self.fw.to_s.empty?
      self.mac_gmid = entity_info['grandmaster-id']
      TEST_INFO.add_warning('Empty entity <grandmaster-id>: ' + self.uid.to_s(16)) if self.mac_gmid.to_s.empty?
      self.config_id = entity_info['current-configuration']
      TEST_INFO.add_warning('Empty entity <config_id>: ' + self.uid.to_s(16)) if self.config_id.to_s.empty?
            
      # Check the AVB status for the current entity and set availability
      self.avbstatus = @avdecc.get_entity_avbstatus(self.uid)
      self.available = true if self.avbstatus['Status'] = 1

      # Check the device type: carbon, pre or virtual
      if self.name.include? "Pre"
        self.type = "pre"
      elsif self.name.include? "Expanded"
        self.type = "virtual"
      elsif self.name.include? "Carbon"
        self.type = "carbon"
      end

      # List all configurations for current entity
      self.configs = @avdecc.list_configurations(self.uid)
      self.current_config = AVBConfig.new(@avdecc, self.uid, self.configs[self.config_id])

    end

    def info

      print "Entity name: "
      puts self.name
      print "ID: "
      puts self.uid.to_s(16)
      print "Firmware Version: "
      puts self.fw
      print "MAC GM ID: "
      puts self.mac_gmid
      print "AVB Status: "
      puts self.avbstatus
      puts "Configs: "
      puts self.configs
      print "Type: "
      puts self.type
      print "Available: "
      puts self.available
      puts ">>>"

    end

  end # class

=begin rdoc
Class for interacting with AVDECC controller
=end

  class AVBController

    attr_reader :name, :available_entities, :mac_gmid, :interface
    attr_writer :name, :available_entities, :mac_gmid, :interface
    def initialize(avdecc, name)

      @avdecc = avdecc
      self.name = name

      ifaces = @avdecc.list_network_interfaces
      raise('No network interfaces were found') if ifaces.empty?
      self.interface = ifaces[0] #may not be correct for controller auto-discovery
      
      print("Creating AVBController... ")
      # res = @avdecc.create_controller(self.interface, self.name)
      # raise('Failed to create controller') if res == false
      # res = @avdecc.create_expanded_controller(self.interface)
      # raise('Failed to create expanded controller') if res == false

      res = @avdecc.discover_expanded_controller
      raise('Failed to discover expanded controller') if res == false

      self.update
      puts "done"

    end

    def update

      self.available_entities = @avdecc.list_available_entities
      raise ('No available entities discovered.') if self.available_entities.empty?
      self.mac_gmid = @avdecc.get_mac_grandmasterid(self.name)
      TEST_INFO.add_warning('Empty controller <mac_gmid>: ' + self.name) if self.mac_gmid.to_s.empty?

    end

    def info

      print "Controller: "
      puts self.name
      print "Interface: "
      puts self.interface
      puts "Available entities: "
      puts self.available_entities
      print "Mac GM ID: "
      puts self.mac_gmid

    end

    def destroy

      print("Destroying AVBController #{self.name}... ")

      begin
        res = @avdecc.destroy_controller(self.name)
        raise('Controller was not destroyed') if res == 0
      rescue Exception => e
        puts e
        raise('Failed to destroy controller')
      end

      puts "done"

    end

  end # class
end # module
