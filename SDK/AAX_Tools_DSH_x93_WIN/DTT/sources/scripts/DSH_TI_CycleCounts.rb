#
#
# Copyright 2013 by Avid Technology, Inc.
#
#

# Created by Pavlo Kharchenko on 15/05/2011

class Array
  def sum
    inject(0) { |sum, x| sum+x }
  end
end

class DSH_TI_CycleCounts < Script

  def self.inputs
    return {
        :millisecs    => [5000],
        :settings_dir => ['none'],
        :plugin_spec  => [["AVID","ChSt","CMTi"]],
        :check_bypass => [false, [true, false]],
        :cache        => ['without-cache', ['with-cache', 'without-cache']],
        :sample_rate  => [96000, [44100, 48000, 88200, 96000, 176400, 192000]],
        :test_mtd     => ['cyclesshared_mtd', ['cycles_mtd', 'cyclesshared_mtd']]
    }
  end

  def self.gui_order
    return [:plugin_spec, :millisecs, :cache, :sample_rate, :settings_dir, :test_mtd, :check_bypass]
  end

  PROP_MAX_INSTANCES_PER_CHIP = 'AAX_eProperty_TI_MaxInstancesPerChip'
  PROP_BUFFEER_LENGTH = 'kTISysProp_AudioBufferLength'
  PROP_INSTANCE = 'kTISysProp_InstanceReqs'
  PROP_INSTANCE_CYCLECOUNT = 'AAX_eProperty_TI_InstanceCycleCount'
  PROP_SHARED_CYCLECOUNT = 'AAX_eProperty_TI_SharedCycleCount'
  FACTORY_DEFAULT = 'factory default'
  MASTER_BYPASS = 'Master Bypass'
  MASTER_BYPASS_IDX = 1
  MASTER_BYPASS_OFF = 0
  MASTER_BYPASS_ON = 1

  def run
    #try to find @presets
    @presets = []
    @presets << Hash[:basename, FACTORY_DEFAULT, :path, FACTORY_DEFAULT]

    if settings_dir != "none"
      dir = Pathname.new(settings_dir)
      raise "Could not find settings_dir: #{settings_dir}" unless dir.exist?

      dir.children(true).each do |file|
        next if (file.extname != ".tfx")
        @presets << Hash[:basename, file.basename.to_s, :path, file.realpath.to_s]
      end
    end

    #try to find target plug-in
    @dsh = DigiShell.new(target)
    @dsh.load_dish("DAE")
    @dsh.init_dae(sample_rate)
    @dsh.acquiredeck([1, sample_rate, "dsp"])

    plugin = {}
    @dsh.run.each do |pi|
      if pi['effect']["#{plugin_spec.inspect}"]
        plugin[:type] = pi['type']
        plugin[:input] = pi['input']
        plugin[:output] = pi['output']
        plugin[:name] = /^(\d+)\.\s+(.+)\s+\[(.+)\]/.match(pi['effect'])[2]
      end
    end

    raise "Could not find target plug-in: #{plugin_spec.inspect}" if plugin.empty?
    raise "Plugin type should be \'berlin\'. Target plug-in type: #{plugin[:type]}" if plugin[:type] != 'berlin'

    @dsh.run(plugin_spec)
    instance = @dsh.getcurrentinstance

    raise "@dsh_DAE failed to instantiate plug-in:#{plugin_spec.inspect}" if instance == -1

    properties = @dsh.getinstanceproperties(instance)

    result_based_on_pi_instances = false

    if properties[PROP_INSTANCE].key?(PROP_MAX_INSTANCES_PER_CHIP)
      max_instances_per_chip = properties[PROP_INSTANCE][PROP_MAX_INSTANCES_PER_CHIP]
      result_based_on_pi_instances = true
    else
      instance_cycles = properties[PROP_INSTANCE][PROP_INSTANCE_CYCLECOUNT]
      shared_cycles = properties[PROP_INSTANCE][PROP_SHARED_CYCLECOUNT]
    end

    @dsh.setenablechipinstancecountlimiting("false")

    @max_cycles_value = Hash[:instance_cc, 0, :shared_cc, 0, :cycless_sum, 0, :per_chip_ic, [], :preset, ""]

    if test_mtd == 'cyclesshared_mtd'
      get_shared_cycles
    else
      get_instance_cycles
    end

    @dsh.close

    test_fail = false
    result_string = ""

    #in case if plug-in has 'AAX_eProperty_TI_InstanceCycleCount' property in describe
    #we build pass/fail criteria based on cycles, otherwise on instances per chip
    if  result_based_on_pi_instances
      result_string = "#{sample_rate} | #{plugin[:name]} #{plugin[:input]}/#{plugin[:output]} Actual: #{get_instances_for_sr(@max_cycles_value[:per_chip_ic])} instances. Expected less than: #{max_instances_per_chip} instances"
      result_string.log_status

      test_fail = true if get_instances_for_sr(@max_cycles_value[:per_chip_ic]) > max_instances_per_chip
    else
      if test_mtd == 'cyclesshared_mtd'
        result_string = "#{sample_rate} | #{plugin[:name]} #{plugin[:input]}/#{plugin[:output]}"
        result_string += "  MaxInstReal = #{@max_cycles_value[:instance_cc]}, MaxShdReal = #{@max_cycles_value[:shared_cc]} | InstDeclared = #{instance_cycles}, ShdDeclared = #{shared_cycles}"
        result_string += " | MAX PerCardInstance: #{@max_cycles_value[:per_chip_ic].inspect} | #{@max_cycles_value[:preset]}"
        result_string.log_status

        max_cycle = [@max_cycles_value[:cycless_sum], @max_cycles_value[:instance_cc] + @max_cycles_value[:shared_cc]].max
      else
        result_string = "#{sample_rate} | #{plugin[:name]} #{plugin[:input]}/#{plugin[:output]} | Expected: #{instance_cycles + shared_cycles} cycles. Actual: #{@max_cycles_value[:instance_cc]}"
        "\n#{result_string}".log_status
        max_cycle = @max_cycles_value[:instance_cc]
      end

      test_fail = true if max_cycle > instance_cycles + shared_cycles
    end

    @dsh.close

    test_fail ? fail(result_string) : pass(result_string)

  rescue Exception => e
    @dsh.close unless @dsh.nil?
    e.backtrace.each { |line| line.log_status }
    fail(e.message)
  end

  def get_shared_cycles
    "Getting the number of cycles using \'cyclesshared\' method".log_status

    @dsh.unload

    @presets.each.with_index do |preset, index|
      if (preset[:basename] != FACTORY_DEFAULT)
        cyclesshared_param = Hash['spec', plugin_spec, 'load_preset', preset[:path]]
      else
        cyclesshared_param = Hash['spec', plugin_spec]
      end

      cyclesshared_output = @dsh.cyclesshared(cyclesshared_param)

      sum = cyclesshared_output[PROP_INSTANCE_CYCLECOUNT] + cyclesshared_output[PROP_SHARED_CYCLECOUNT]

      #searching max values for :instance_cc, :shared_cc, :cycless_sum, :per_chip_ic
      if cyclesshared_output[PROP_INSTANCE_CYCLECOUNT] > @max_cycles_value[:instance_cc]
        @max_cycles_value[:instance_cc] = cyclesshared_output[PROP_INSTANCE_CYCLECOUNT]
      end

      if cyclesshared_output[PROP_SHARED_CYCLECOUNT] > @max_cycles_value[:shared_cc]
        @max_cycles_value[:shared_cc] = cyclesshared_output[PROP_SHARED_CYCLECOUNT]
      end

      if sum > @max_cycles_value[:cycless_sum]
        @max_cycles_value[:cycless_sum] = sum
      end

      if cyclesshared_output['PerChipInstanceCounts'].sum > @max_cycles_value[:per_chip_ic].sum
        @max_cycles_value[:per_chip_ic] = cyclesshared_output['PerChipInstanceCounts']
        @max_cycles_value[:preset] = preset[:basename]
      end

      #Logging current values
      log_string = "#{index+1}. Instance CC: #{cyclesshared_output[PROP_INSTANCE_CYCLECOUNT]} | "
      log_string += "Shared CC: #{cyclesshared_output[PROP_SHARED_CYCLECOUNT]} | "
      log_string += "Sum: #{sum} | CorrelCoeff:#{cyclesshared_output['CorrelationCoeff']} | "
      log_string += "perChip: #{cyclesshared_output['PerChipInstanceCounts'].inspect} | #{preset[:basename]}"
      log_string.log_status
    end
  end

  def get_instance_cycles
    "Getting the number of cycles using \'cycles\' method".log_status
    @presets.each.with_index do |preset, index|
      @dsh.load_settings(preset[:path]) if preset[:basename] != FACTORY_DEFAULT
      cycles = @dsh.cycles([millisecs, cache])['max']

      log_string = "#{index+1}. cycles: #{cycles} | "

      if check_bypass
        "Enabling Master Bypass" .log_status
        @dsh.control(MASTER_BYPASS_IDX, MASTER_BYPASS_ON)
        bypassed_cycles = @dsh.cycles([millisecs, cache])['max']
        cycles = bypassed_cycles if bypassed_cycles > cycles
        log_string += "Bypassed Cycles: #{bypassed_cycles} | "
        @dsh.control(MASTER_BYPASS_IDX, MASTER_BYPASS_OFF)
      end

      log_string += "#{preset[:basename]}"
      log_string.log_status


      if cycles > @max_cycles_value[:instance_cc]
        @max_cycles_value[:instance_cc] = cycles
        @max_cycles_value[:preset] = preset[:basename]
      end
    end
  end

  def get_instances_for_sr(per_chip_ic)
    case sample_rate
      when 44100, 48000
        per_chip_ic[0]
      when 88200, 96000
        per_chip_ic[1]
      when 176400, 192000
        per_chip_ic[2]
      else
        raise "Unknown sample rate: #{sample_rate}"
    end
  end
end
