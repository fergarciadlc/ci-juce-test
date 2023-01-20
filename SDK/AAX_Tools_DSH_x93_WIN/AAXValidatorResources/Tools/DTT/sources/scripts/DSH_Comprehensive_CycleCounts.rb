#
#  DSH_Comprehensive_CycleCounts.rb
#  DTT
#
#  Created by Sergii Dekhtiarenko on 25/02/10.
#  Copyright 2014 by Avid Technology, Inc.
#

class DSH_Comprehensive_CycleCounts < Script

  def self.inputs
    return {
        :bundle_path  => ['/Volumes/path/moogerfooger RM.aaxplugin']
    }
  end

  SAMPLE_RATES = [48000, 96000, 192000]
  ERR_BAD_PRESET = 'Bad plugin chunk'
  FACTORY_DEFAULT = 'factory default'
  MASTER_BYPASS = 'Master Bypass'
  MASTER_BYPASS_IDX = 1
  MASTER_BYPASS_OFF = 0
  MASTER_BYPASS_ON = 1

  def run
    if target.exist_on_target?(bundle_path)
      real_path = target.real_path(bundle_path)
    else
      return cancel("Could NOT find specified bundle: #{bundle_path}")
    end

    presets_dir = Pathname.new(real_path) + 'Contents' + 'Factory Presets'

    @bad_results = []

    #searching presets
    @presets = []
    @presets.push(Hash[:basename, FACTORY_DEFAULT, :path, FACTORY_DEFAULT])

    presets_dir.exist? ? grab_presets(presets_dir) : 'Factory Presets were NOT found'.warn

    #trying to grab effects' IDs
    @dsh = DigiShell.new(target)

    @dsh.enable_trace_facility(["DTF_AAXH_CLIENT", "DTP_LOWEST"])
    @dsh.load_dish("aaxh")
    @dsh.teardown
    @dsh.init('debug_max')

    plugin_id = @dsh.loadpi(real_path)['pluginID']

    effects = []
    listeffects = @dsh.listeffects(plugin_id)['effectIDs']

    listeffects.each do |rough_effect|
      effect = {}

      effect[:effect_id] = rough_effect['effectID']
      effect[:description] = @dsh.grab_effect_info(plugin_id, rough_effect['effectIdx'])

      effects.push(effect)
    end

    @dsh.close

    #Checking that system is configured for the dsp plug-ins
    @dsh = DigiShell.new(target)
    @dsh.load_dish('DAE')
    @dsh.init_dae({'ignore_defaults' => true})
    @dsh.acquiredeck  #System will acquire HDX if it available
    deck_type = @dsh.getdeckproperties['deck_type']

    return cancel('Make sure that HDX card and driver are installed properly') if deck_type != 'DSP'
    
    @dsh.close

    #running test itself
    effects.each do |effect|
      AAXHDishUtils::SAMPLE_RATES_BASE.values.each do |sample_rate|
        configure_dae(sample_rate, real_path)
        effect[:description][:dsp_types].each do |type|
          next unless type[:cycles][sample_rate]

          sub_result = self.create_sub_result
          sub_result.effect_id = effect[:effect_id]

          manu_id = type[:manu_id].to_uint32
          prod_id = type[:prod_id].to_uint32
          ti_id   = type[:ti_id].to_uint32

          sub_result.triad_id = [manu_id, prod_id, ti_id]
          sub_result.log_test_config({:sample_rate => sample_rate})

          sub_result.add_log("#{effect[:effect_id]}: #{type[:input_sf]}/#{type[:out_sf]}. #{sample_rate} Hz")

          measure_cycles_for_presets(type, sample_rate, [], sub_result)
          measure_cycles_with_adjust_controls(type, sample_rate, 1, sub_result)
        end
      end
    end #effect

    @dsh.close if @dsh

    if @bad_results.empty?
      return pass("Real cycles don't exceed declared cycles.")
    else
      return fail("Real cycles exceed declared cycles.")
    end

  end #run

  def grab_presets(folder)
    folder.children(true).each do |item|
      grab_presets(item) if item.directory?

      next if (item.extname != '.tfx')
      @presets.push(Hash[:basename, item.basename.to_s, :path, item.realpath.to_s])
    end
  end

  def measure_cycles_for_presets(type, sample_rate, audio_buffers = [], sub_result)
    spec = Array[type[:manu_id], type[:prod_id], type[:ti_id]]

    max_cycles_value = Hash[:instance_cc, 0, :shared_cc, 0, :preset_instance, "", :preset_shared, ""]
    decl_instance_cc, decl_shared_cc = type[:cycles][sample_rate]

    @presets.each do |preset|
      begin
        cmd_parameters = {}

        cmd_parameters['spec'] = spec
        cmd_parameters['audio_buffers'] = audio_buffers unless audio_buffers.empty?
        cmd_parameters['load_preset'] = preset[:path] if (preset[:basename] != FACTORY_DEFAULT)

        cyclesshared_output = @dsh.cyclesshared(cmd_parameters)

        # cyclesshared_output = {}
        # cyclesshared_output[AAXHDishUtils::PROP_INSTANCE_CC] = 10
        # cyclesshared_output[AAXHDishUtils::PROP_SHARED_CC] = 0

        #searching max values for :instance_cc, :shared_cc
        if cyclesshared_output[AAXHDishUtils::PROP_INSTANCE_CC] > max_cycles_value[:instance_cc]
          max_cycles_value[:instance_cc] = cyclesshared_output[AAXHDishUtils::PROP_INSTANCE_CC]
          max_cycles_value[:preset_instance] = preset[:basename]
        end

        if cyclesshared_output[AAXHDishUtils::PROP_SHARED_CC] > max_cycles_value[:shared_cc]
          max_cycles_value[:shared_cc] = cyclesshared_output[AAXHDishUtils::PROP_SHARED_CC]
          max_cycles_value[:preset_shared] = preset[:basename]
        end

      rescue Exception => err
        sub_result.add_log("ERROR: " + err.message)
        next if err.message.include?(ERR_BAD_PRESET)
        raise err
      end
    end # each |preset|

    exceeded = false

    if (max_cycles_value[:instance_cc] > decl_instance_cc || max_cycles_value[:shared_cc] > decl_shared_cc)
      exceeded = true
      node = sub_result.create_tree_node("Failures")
    else
      node = sub_result.create_tree_node("Worst-Case")
    end

    exceeded_instance_cycles = {}
    exceeded_instance_cycles['Instance Cycles'] = {}
    exceeded_instance_cycles['Instance Cycles']['Preset'] = max_cycles_value[:preset_instance]
    exceeded_instance_cycles['Instance Cycles']['Declared Instance Cycles'] = decl_instance_cc
    exceeded_instance_cycles['Instance Cycles']['Real Instance Cycles'] = max_cycles_value[:instance_cc]

    sub_result.add_data_to_node(exceeded_instance_cycles, node)
    @bad_results.push(exceeded_instance_cycles) if exceeded

    exceeded_shared_cycles = {}
    exceeded_shared_cycles['Shared Cycles'] = {}
    exceeded_shared_cycles['Shared Cycles']['Preset'] = max_cycles_value[:preset_shared]
    exceeded_shared_cycles['Shared Cycles']['Declared Shared Cycles'] = decl_shared_cc
    exceeded_shared_cycles['Shared Cycles']['Real Shared Cycles'] = max_cycles_value[:shared_cc]

    sub_result.add_data_to_node(exceeded_shared_cycles, node)
    @bad_results.push(exceeded_shared_cycles) if exceeded
  end

  def measure_cycles_with_adjust_controls(type, sample_rate, attempts = 1, audio_buffers = [], sub_result)
    spec = []
    spec.push(type[:manu_id])
    spec.push(type[:prod_id])
    spec.push(type[:ti_id])

    max_cycles_value = Hash[:instance_cc, 0, :shared_cc, 0]
    decl_instance_cc, decl_shared_cc = type[:cycles][sample_rate]

    instance_cc = []
    shared_cc = []

    input_params = Hash.new
    input_params['spec'] = spec
    input_params['adjust_controls'] = true
    input_params['audio_buffers'] = audio_buffers unless audio_buffers.empty?

    attempts.times do
      cyclesshared_output = @dsh.cyclesshared(input_params)

      # cyclesshared_output = {}
      # cyclesshared_output[AAXHDishUtils::PROP_INSTANCE_CC] = 10
      # cyclesshared_output[AAXHDishUtils::PROP_SHARED_CC] = 0

      instance_cc.push(cyclesshared_output[AAXHDishUtils::PROP_INSTANCE_CC])
      shared_cc.push(cyclesshared_output[AAXHDishUtils::PROP_SHARED_CC])

      #searching max values for :instance_cc, :shared_cc
      if cyclesshared_output[AAXHDishUtils::PROP_INSTANCE_CC] > max_cycles_value[:instance_cc]
        max_cycles_value[:instance_cc] = cyclesshared_output[AAXHDishUtils::PROP_INSTANCE_CC]
      end

      if cyclesshared_output[AAXHDishUtils::PROP_SHARED_CC] > max_cycles_value[:shared_cc]
        max_cycles_value[:shared_cc] = cyclesshared_output[AAXHDishUtils::PROP_SHARED_CC]
      end
    end #attempts

    exceeded = false

    if (max_cycles_value[:instance_cc] > decl_instance_cc || max_cycles_value[:shared_cc] > decl_shared_cc)
      exceeded = true
      node = sub_result.create_tree_node("Failures")
    else
      node = sub_result.create_tree_node("Worst-Case")
    end

    exceeded_cycles = {}
    exceeded_cycles['Randomly adjust controls'] = {}
    exceeded_cycles['Randomly adjust controls']['Declared Instance Cycles'] = decl_instance_cc
    exceeded_cycles['Randomly adjust controls']['Real Instance Cycles'] = max_cycles_value[:instance_cc]

    exceeded_cycles['Randomly adjust controls']['Declared Shared Cycles'] = decl_shared_cc
    exceeded_cycles['Randomly adjust controls']['Real Shared Cycles'] = max_cycles_value[:shared_cc]

    sub_result.add_data_to_node(exceeded_cycles, node)

    @bad_results.push(exceeded_cycles) if exceeded

  end

  def configure_dae(sample_rate, plugin_path)
    @dsh.close if (@dsh and !@dsh.closed?)

    @dsh = DigiShell.new(target)

    @dsh.load_dish('DAE')
    @dsh.init_dae({'sample_rate' => sample_rate, 'ignore_defaults' => true })
    @dsh.acquiredeck([1, sample_rate, 'DSP'])
    @dsh.loadpi(plugin_path)
  end

  def get_channels(stem_format)
    case stem_format
      when 'mono', 'Any'
        1
      when 'stereo'
        2
      when 'LCR'
        3
      when 'LCRS', 'Quad'
        4
      when '5_0'
        5
      when '5_1', '6_0'
        6
      when '6_1', '7_0', '7_0_DTS', '7_0_SDDS'
        7
      when '7_1', '7_1_DTS', '7_1_SDDS'
        8
      else
        raise "Unknown stem-format: #{channel}"
    end
  end
end
