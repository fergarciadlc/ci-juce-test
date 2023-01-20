#
#
# Copyright 2013 by Avid Technology, Inc.
#
#
# Created by Pavlo Kharchenko on 15/05/2011

class DSH_TI_CycleCounts < Script

  def self.inputs
    return {
        :plugin_name  => [],
        :millisecs    => [5000], # is used for dsh.cycles method
        :settings_dir => ['none'],
        :plugin_spec  => [["AVID","ChSt","CMTi"]], # Default is DSP Channel Strip mono/mono
        :check_bypass => [false, [true, false]],
        :cache        => ['without-cache', ['with-cache', 'without-cache']],
        :sample_rate  => [96000, [44100, 48000, 88200, 96000, 176400, 192000]],
        :test_mtd     => ['cyclesshared_mtd', ['cycles_mtd', 'cyclesshared_mtd']],
        :threshold    => [0.05],
        :run_remotely => true
    }
  end

  def self.log_annexes
    [:plugin_name, :sample_rate]
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

    if run_remotely
      @rdsh = SSHConnect.new
      @rdsh.run_remotely(@rdsh, @dsh)
    end

    @dsh.load_dish("DAE")
    @dsh.init_dae(sample_rate)
    @dsh.acquiredeck([1, sample_rate, "dsp"])

    found_pi = @dsh.findpi(plugin_spec.inspect)
    raise "Could not find target plug-in: #{plugin_spec.inspect}" if found_pi.instance_of?(String)

    plugin = {}
    idx = found_pi[0]['type'].eql?('berlin') ? 0 : 1
    plugin[:type] = found_pi[idx]['type']
    plugin[:input] = found_pi[idx]['input']
    plugin[:output] = found_pi[idx]['output']
    plugin[:name] = /^(\d+)\.\s+(.+)\s+\[(.+)\]/.match(found_pi[idx]['effect'])[2]

    raise "Plugin type should be \'berlin\'. Target plug-in type: #{plugin[:type]}" if plugin[:type] != 'berlin'

    @dsh.run(plugin_spec)
    instance = @dsh.getcurrentinstance

    raise "@dsh_DAE failed to instantiate plug-in:#{plugin_spec.inspect}" if instance == -1

    properties = @dsh.getinstanceproperties(instance)

    based_on_pi_instances = false

    if properties[PROP_INSTANCE].key?(PROP_MAX_INSTANCES_PER_CHIP)
      based_on_pi_instances = true
      declared_instances_per_chip = properties[PROP_INSTANCE][PROP_MAX_INSTANCES_PER_CHIP]
    else
      declared_instance_cycles = properties[PROP_INSTANCE][PROP_INSTANCE_CYCLECOUNT]
      declared_shared_cycles = properties[PROP_INSTANCE][PROP_SHARED_CYCLECOUNT]
      declared_sum = declared_instance_cycles + declared_shared_cycles
    end

    @dsh.setenablechipinstancecountlimiting("false")

    @max_cycles_value = {:instance_cycles => 0, :instance_cycles_tfx => "", :shared_cycles => 0,
                         :shared_cycles_tfx => "", :cycless_sum => 0, :per_chip_ic => [], :chip_ic_tfx => ""}

    if test_mtd == 'cyclesshared_mtd'
      get_shared_cycles
    else
      get_instance_cycles
    end

    @dsh.close

    test_fail = true

    #in case if plug-in has 'AAX_eProperty_TI_InstanceCycleCount' property in describe
    #we build pass/fail criteria based on cycles, otherwise on instances per chip
    if  based_on_pi_instances
      result_string = "#{sample_rate} | #{plugin[:name]} #{plugin[:input]}/#{plugin[:output]} Actual: #{get_instances_for_sr(@max_cycles_value[:per_chip_ic])} instances. Expected less than: #{declared_instances_per_chip} instances"
      result_string.log_status

      test_fail = false if get_instances_for_sr(@max_cycles_value[:per_chip_ic]) <= declared_instances_per_chip
    else
      if test_mtd == 'cyclesshared_mtd'

        result_string = "#{sample_rate} | #{plugin[:name]} #{plugin[:input]}/#{plugin[:output]}"

        instance_cycles_result = " MaxInstReal = #{@max_cycles_value[:instance_cycles]} | InstDeclared = #{declared_instance_cycles}"
        shared_cycles_result = " MaxShdReal = #{@max_cycles_value[:shared_cycles]} | ShdDeclared = #{declared_shared_cycles}"
        sum_cycles_result = " MaxSumReal #{@max_cycles_value[:cycless_sum]} | SumDeclared = #{declared_sum}"

        result_string += sum_cycles_result
        result_string += instance_cycles_result + " #{@max_cycles_value[:instance_cycles_tfx]}."
        result_string += shared_cycles_result + " #{@max_cycles_value[:shared_cycles_tfx]}."

        if (declared_sum >= @max_cycles_value[:cycless_sum]) and (declared_instance_cycles >= @max_cycles_value[:instance_cycles])
          test_fail = false
        end

      else
        result_string = "#{sample_rate} | #{plugin[:name]} #{plugin[:input]}/#{plugin[:output]} | Expected: #{declared_instance_cycles + declared_shared_cycles} cycles. Actual: #{@max_cycles_value[:instance_cycles]}"
        result_string.log_status
        test_fail = false if (@max_cycles_value[:instance_cycles] <= (declared_instance_cycles + declared_shared_cycles))
      end
    end

    @dsh.close

    test_fail ? fail(result_string) : pass(result_string)

  rescue Exception => e
    @dsh.close unless @dsh.nil?
    e.backtrace.each { |line| line.log_verbose}
    fail(e.message)
  end

  def get_shared_cycles
    "Getting the number of cycles using \'cyclesshared\' method".log_status

    @dsh.unload

    @presets.each.with_index do |tfx, index|
      if (tfx[:basename] != FACTORY_DEFAULT)
        params = Hash['spec', plugin_spec, 'load_preset', tfx[:path]]
      else
        params = Hash['spec', plugin_spec]
      end

      cyclesshared_output = @dsh.cyclesshared(params)

      #searching max values for :instance_cc, :shared_cc, :cycless_sum, :per_chip_ic
      if cyclesshared_output[PROP_INSTANCE_CYCLECOUNT] > @max_cycles_value[:instance_cycles]
        @max_cycles_value[:instance_cycles] = cyclesshared_output[PROP_INSTANCE_CYCLECOUNT]
        @max_cycles_value[:instance_cycles_tfx] = tfx[:basename]
      end

      if cyclesshared_output[PROP_SHARED_CYCLECOUNT] > @max_cycles_value[:shared_cycles]
        @max_cycles_value[:shared_cycles] = cyclesshared_output[PROP_SHARED_CYCLECOUNT]
        @max_cycles_value[:shared_cycles_tfx] = tfx[:basename]
      end

      if (@max_cycles_value[:instance_cycles] + @max_cycles_value[:shared_cycles]) > @max_cycles_value[:cycless_sum]
        @max_cycles_value[:cycless_sum] = @max_cycles_value[:instance_cycles] + @max_cycles_value[:shared_cycles]
      end

      if cyclesshared_output['PerChipInstanceCounts'].sum > @max_cycles_value[:per_chip_ic].sum
        @max_cycles_value[:per_chip_ic] = cyclesshared_output['PerChipInstanceCounts']
        @max_cycles_value[:chip_ic_tfx] = tfx[:basename]
      end

      #Logging current values
      log_string = "#{index+1}. Instance CC: #{cyclesshared_output[PROP_INSTANCE_CYCLECOUNT]} | "
      log_string += "Shared CC: #{cyclesshared_output[PROP_SHARED_CYCLECOUNT]} | "
      log_string += "CorrelCoeff:#{cyclesshared_output['CorrelationCoeff']} | "
      log_string += "perChip: #{cyclesshared_output['PerChipInstanceCounts'].inspect} | #{tfx[:basename]}"
      log_string.log_status
    end
  end

  def get_instance_cycles
    "Getting the number of cycles using \'cycles\' method".log_status
    @presets.each.with_index do |tfx, index|
      @dsh.load_settings(tfx[:path]) if tfx[:basename] != FACTORY_DEFAULT
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

      log_string += "#{tfx[:basename]}"
      log_string.log_status


      if cycles > @max_cycles_value[:instance_cycles]
        @max_cycles_value[:instance_cycles] = cycles
        @max_cycles_value[:instance_cycles_tfx] = tfx[:basename]
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
