#
#  AAXHDishUtils.rb
#  DishTestTool
#
#  Created by Sergii Dekhtiarenko
#  Copyright 2014 by Avid Technology, Inc.
#
module DishTestTool
  module AAXHDishUtils
    PROP_MAX_INSTANCES_PER_CHIP = 'AAX_eProperty_TI_MaxInstancesPerChip'
    PROP_BUFFEER_LENGTH = 'kTISysProp_AudioBufferLength'
    PROP_INSTANCE = 'kTISysProp_InstanceReqs'

    PROP_INSTANCE_CC = 'AAX_eProperty_TI_InstanceCycleCount'
    PROP_SHARED_CC = 'AAX_eProperty_TI_SharedCycleCount'

    PROP_MANU_ID = 'AAX_eProperty_ManufacturerID'
    PROP_PROD_ID = 'AAX_eProperty_ProductID'
    PROP_TI_ID = 'AAX_eProperty_PlugInID_TI'
    PROP_NV_ID = 'AAX_eProperty_PlugInID_Native'
    PROP_AS_ID = 'AAX_eProperty_PlugInID_AudioSuite'
    PROP_OUT_SF = 'AAX_eProperty_OutputStemFormat'
    PROP_INP_SF = 'AAX_eProperty_InputStemFormat'

    PROP_HYBRID_INP_SF = 'AAX_eProperty_HybridInputStemFormat'
    PROP_HYBRID_OUT_SF = 'AAX_eProperty_HybridOutputStemFormat'

    PROP_METER_TYPE = 'AAX_eProperty_Meter_Type'
    PROP_METER_ORIENT = 'AAX_eProperty_Meter_Orientation'

    PROP_SAMPLE_RATE = 'AAX_eProperty_SampleRate'

    SAMPLE_RATES = {
        :'44.1 kHz'   => 44100,
        :'48 kHz'     => 48000,
        :'88.2 kHz'   => 88200,
        :'96 kHz'     => 96000,
        :'176.4 kHz'  => 176400,
        :'192 kHz'    => 192000
    }

    SAMPLE_RATES_BASE = {
        :'48 kHz'     => 48000,
        :'96 kHz'     => 96000,
        :'192 kHz'    => 192000
    }

    PLATFORM = {
        :native => 'native',
        :dsp => 'dsp'
    }

    STEM_FORMAT = {
        :mono       => 'mono',
        :stereo     => 'stereo',
        :lcr        => 'lcr',
        :lcrs       => 'lcrs',
        :quad       => 'quad',
        :'5_0'      => '5p0',
        :'5_1'      => '5p1',
        :'6_0'      => '6p0',
        :'6_1'      => '6p1',
        :'7_0_sdds' => '7p0sdds',
        :'7_1_sdds' => '7p1sdds',
        :'7_0_dts'  => '7p0dts',
        :'7_1_dts'  => '7p1dts'
    }

    PLUGIN_CATEGORY = {
        0x00000000 => :'None',
        0x00000001 => :'EQ',             #Equalization
        0x00000002 => :'Dynamics',       #Compressor, expander, limiter, etc.
        0x00000004 => :'Pitch Shift',    #Pitch processing
        0x00000008 => :'Reverb',         #Reverberation and room/space simulation
        0x00000010 => :'Delay',          #Delay and echo
        0x00000020 => :'Modulation',     #Phasing, flanging, chorus, etc.
        0x00000040 => :'Harmonic',       #Distortion, saturation, and harmonic enhancement
        0x00000080 => :'NoiseReduction', #Noise reduction
        0x00000100 => :'Dither',         #Dither, noise shaping, etc.
        0x00000200 => :'Sound Field',    #Pan, auto-pan, upmix and downmix, and surround handling
        0x00000400 => :'HWGenerators',   #Fixed hardware audio sources such as SampleCell
        0x00000800 => :'SWGenerators',   #Virtual instruments, metronomes, and other software audio sources
        0x00001000 => :'WrappedPlugin',  #All plug-ins wrapped by a thrid party wrapper (i.e. VST to RTAS wrapper), except for VI plug-ins which should be mapped to AAX_PlugInCategory_SWGenerators
        0x00002000 => :'Effect',         #Special effects
        0x00004000 => :'Example'         #SDK example plug-ins \compatibility \ref AAX_ePlugInCategory_Example is compatible with Pro Tools 11 and higher. Effects with this category will not appear in Pro Tools 10.
    }

    CONVENTIONS = {
        PROP_SAMPLE_RATE            => [:to_sample_rate, :sample_rate],

        PROP_PROD_ID                => [:to_characters, :prod_id],
        PROP_MANU_ID                => [:to_characters, :manu_id],
        PROP_TI_ID                  => [:to_characters, :ti_id],
        PROP_NV_ID                  => [:to_characters, :native_id],
        PROP_AS_ID                  => [:to_characters, :as_id],

        PROP_OUT_SF                 => [:to_stem_format, :out_sf],
        PROP_INP_SF                 => [:to_stem_format, :input_sf],
        PROP_HYBRID_INP_SF          => [:to_stem_format, :hybrid_input_sf],
        PROP_HYBRID_OUT_SF          => [:to_stem_format, :hybrid_out_sf],

        PROP_MAX_INSTANCES_PER_CHIP => [nil, :max_inst_per_chip],
        PROP_INSTANCE_CC            => [nil, :instance_cc],
        PROP_SHARED_CC              => [nil, :shared_cc],

        PROP_METER_TYPE             => [nil, :type],
        PROP_METER_ORIENT           => [nil, :orientation]
    }

    def grab_effect_info(plugin_id, effect_id)
      info = {}
      info[:name] = nil

      #{plugin: plugin_value, effect: effect_value, stringformat: stringformat_value}
      description = self.getdescription(Hash['plugin', plugin_id, 'effect', effect_id, 'stringformat', 'yaml'])
      description = description['effectDescription']['plugin_descriptor'] #changing the root

      info[:name] = description['name']
      info[:category] = to_plugin_category(description['category_mask'])
      info[:page_table_file] = description['page_table_file'] if description['page_table_file']

      #gathering info about effect meters
      if description['meters']
        info[:effect_meters] = []

        effect_meters = description['meters']['meter']
        effect_meters = [effect_meters] if effect_meters.instance_of?(Hash)

        effect_meters.each do |meter|
          meter_info = {}
          meter_info[:name] = meter['name']
          meter_info[:id] = meter['fourchar_id']
          meter['property_map']['property'].each do |prop|
            extract_property(prop, meter_info)
          end
          info[:effect_meters].push(meter_info)
        end
      end

      #gathering info about ManufactureID and ProductID
      if description['property_map']
        description['property_map']['property'].each do |prop|
          next unless prop.instance_of?(Hash)
          extract_property(prop, info)
        end
      end

      return info unless description['components'] #for pure AS plug-ins we don't have 'components'

      dsp_types = Set.new
      native_types = Set.new

      #changing root
      description = description['components']['component_descriptor']
      description = [description] if description.instance_of?(Hash)

      description.each do |component|
        context_meters = []
        #gathering info about context meter
        if component['context_descriptor'] && component['context_descriptor']['meters']
          meters = component['context_descriptor']['meters']['meter']
          meters = [meters] if meters.instance_of?(Hash)
          meters.each do |meter|
            context_meters.push(meter['fourchar_id'])
          end
        end

        next unless (component['procs'] and component['procs']['proc_set']) #AAXTOOL-687

        proc_set_list = component['procs']['proc_set']
        proc_set_list = [proc_set_list] if proc_set_list.instance_of?(Hash)
        proc_set_list.each do |proc_set|
          proc_set['proc_descriptor'].each do |descriptor|
            next unless descriptor.has_key?('properties')
            sub_collector = {}
            sub_collector[:context_meters] = context_meters unless context_meters.empty?

            descriptor['properties']['property_map']['property'].each do |property|
              next unless property.instance_of?(Hash)
              extract_property(property, sub_collector)
            end #property

            #Updating :prod_id and :manu_id
            sub_collector[:prod_id] = info[:prod_id] if sub_collector[:prod_id].nil?
            sub_collector[:manu_id] = info[:manu_id] if sub_collector[:manu_id].nil?

            # if we didn't catch AAX_eProperty_SampleRate property that mean that type available on all sample rates
            sub_collector[:sample_rate] ||= SAMPLE_RATES.values

            if descriptor['dll'][/\.dll$/i]
              ids = dsp_types
              key = :ti_id

              sub_collector[:cycles] = {}
              inst_cc, shar_cc = sub_collector[:instance_cc], sub_collector[:shared_cc]

              sub_collector[:sample_rate].each { |s_r| sub_collector[:cycles][s_r] = [inst_cc, shar_cc] }
              [:instance_cc, :shared_cc, :native_id, :as_id].each {|k| sub_collector.delete(k)}
            else
              ids = native_types
              key = :native_id

              [:instance_cc, :shared_cc, :ti_id].each {|k| sub_collector.delete(k)}
            end

            merge_element = ids.select {|id| id[key] == sub_collector[key]}.first

            if merge_element
              merge_element[:sample_rate] = merge_element[:sample_rate] + sub_collector[:sample_rate]
              merge_element[:cycles].merge!(sub_collector[:cycles]) if (merge_element[:cycles] && sub_collector[:cycles])
            else
              ids.add(sub_collector)
            end
          end
        end
      end

      info[:dsp_types] = dsp_types unless dsp_types.empty?
      info[:native_types] = native_types unless native_types.empty?

      log_description(info, :log_verbose)

      return info
    end #grab_effect_info

    def to_sample_rate(int)
      string = int.to_s(2).rjust(6, '0').reverse.split('')

      sample_rate = []

      0.upto(5) do |bit|
        sample_rate.push(SAMPLE_RATES.values[bit]) if string[bit] == '1'
      end

      return sample_rate
    end

    def to_plugin_category(mask)
      string = mask.to_s.rjust(16, '0').reverse.split('')
      category = []

      0.upto(16) do |bit|
        category.push(PLUGIN_CATEGORY.values[bit+1]) if string[bit] == '1'
      end

      category.push(PLUGIN_CATEGORY[0x00000000]) if category.empty?

      return category
    end

=begin
	file AAX_Enums.h
=end
    def to_stem_format(int)
      int = (int & 0xFFFF0000) >> 16
      case int
        when 0 then 'mono'
        when 1 then 'stereo'
        when 2 then 'LCR'
        when 3 then 'LCRS'
        when 4 then 'Quad'
        when 5 then '5_0'
        when 6 then '5_1'
        when 7 then '6_0'
        when 8 then '6_1'
        when 9 then '7_0_SDDS'
        when 10 then '7_1_SDDS'
        when 11 then '7_0_DTS'
        when 12 then '7_1_DTS'
        when -1 then 'Any'
      end
    end

    def to_characters(int)
      return [int].pack('N')
    end

    def generate_signal(channels = 1, nsamples = 44100, sample_rate = 44100, frequency = 100, type = :sin_wave)
      buffers = []
      samples = []

      if (type == :sin_wave)
        period = sample_rate/frequency.to_f
        inc = 1.0 / period

        period.to_i.times { |i| samples.push(Math.sin(2 * Math::PI * inc*(i+2)))}
      elsif(type == :square)
        # nsamples
      elsif (type == :white_noise)
        # nsamples
      end

      channels.times do |channel|
        float_blob = samples.pack("e*")
        byte_array = float_blob.unpack("C*")

        #genarating mock array to get future size for buffer
        mock_array = Array.new(nsamples, 10).pack("e*")
        mock_byte_array = mock_array.unpack("C*")

        buffer = self.bnew(mock_byte_array.size)['buffer_ref']
        self.bfill(buffer, {'pattern' => byte_array})

        buffers.push(buffer)
      end

      return buffers
    end #generte_signal

    def extract_property(property, extracted = {})
      desired_keys = CONVENTIONS.keys & property.values
      desired_keys.each do |key|
        prop_key, prop_method = CONVENTIONS[key][1], CONVENTIONS[key][0]
        extracted[prop_key] = prop_method.nil? ? property['val_literal'] : send(prop_method, property['val_literal'])
      end #desired_keys
    end #extract_proprty

    def log_description(description, log_method = :log_status)
      description.each do |key, value|
        if !value.is_a?(Enumerable)
          "#{key}: #{value}".send(log_method)
        else
          "#{key}: ".send(log_method)
          value.each { |sub_val| "\t#{sub_val}".send(log_method)}
        end
      end
    end #log_description
  end #AAXHDishUtils
end #DishTestTool