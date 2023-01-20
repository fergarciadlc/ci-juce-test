#
#  DSH_SigCancellation.rb
#  DTT
#
#  Created by Artem Ustinov on 1/11/10.
#  Copyright 2013 by Avid Technology, Inc.
#

class DSH_SigCancellation < Script

  def self.inputs
    return {
      :plugin_name        => [],
      :original_plugin    => [["Digi", "dn3d", "DMRT"]],
      :plugin_to_test     => [["Digi", "dn3d", "d3mm"]],
      :sample_rate        => [44100,[44100,48000,88200,96000,176400,192000]],
      :test_file          => ['Audio file in .wav format with full path'],
      :path_to_tfx        => ['none'],
      :threshold          => [-96],
      :path_to_orig_pt    => ["none"],
      :factory_default    => [false,[true,false]],
    }
  end

  def self.log_annexes
    [:plugin_name, :sample_rate]
  end

  ERR_BAD_PRESET = "Settings file has invalid length"
  CANCELLATION_MINIMUM = -400
  FACTORY_DEFAULT = 'factory default'
  LOCAL_HOST = "127.0.0.1"
  MAX_ATTEMPTS = 3
  BERLIN = "berlin"
  RUBY = "ruby"
  WAV = ".wav"
  BUF = ".buf"

  def run
    pre_flight

    default_build_folder = target.build_folder.to_s

    @dsh = DigiShell.new(target)
    @dsh.load_dish("DAE")

    plugin = @dsh.findpi(plugin_to_test.inspect)[0]
    raise "Could not find specified plugin: #{plugin_to_test.inspect}" unless plugin.instance_of?(Hash)
    target_plugin = get_description(plugin)

    plugin = @dsh.findpi(original_plugin.inspect)[0]
    raise "Could not find specified plugin: #{original_plugin.inspect}" unless plugin.instance_of?(Hash)
    legacy_plugin = get_description(plugin)

    @dsh.close

    if  ((legacy_plugin[:input] != target_plugin[:input]) or (legacy_plugin[:output] != target_plugin[:output]))
      raise "Tested plu-gin's have different stem-formats. First plug-in: #{legacy_plugin.inspect}. Second plug-in: #{target_plugin.inspect}"
    end

    title_info = "#{legacy_plugin[:name]} #{legacy_plugin[:input_format]}/#{legacy_plugin[:output_format]} | #{sample_rate} kHz"
    title_info.log_status

    "\n\n".log_status

    max_result_preset = ""
    max_result = CANCELLATION_MINIMUM

    @presets.each.with_index do |preset, idx|
      begin
        buffers_legacy_plugin = []
        buffers_target_plugin = []
        @buffers = []

        target.build_folder = default_build_folder
        buffers_target_plugin = process_signal(target_plugin, preset)
        @buffers += buffers_target_plugin

        target.build_folder = path_to_orig_pt if path_to_orig_pt != 'none'
        buffers_legacy_plugin = process_signal(legacy_plugin, preset)
        @buffers += buffers_legacy_plugin

        result = compare_buffers(buffers_legacy_plugin, buffers_target_plugin)

        if result > max_result
          max_result_preset = preset[:basename].to_s
          max_result = result
        end

      rescue Exception => err
        if err.to_s.include?(ERR_BAD_PRESET)
          remove_files(@buffers)
          next
        else
          raise err
        end
      end
      "\t#{idx+1}.\t#{result} dB\t#{preset[:basename]}".log_status
      remove_files(@buffers)
    end

    result_message =  "#{title_info} #{max_result} dB\t#{max_result_preset}\t#{@audio.basename}"
    result_message.log_status

    if max_result > threshold
      return fail(result_message)
    else
      return pass(result_message)
    end

  rescue Exception => err
    err.backtrace.each{|line| line.log_verbose}
    return fail(err.to_s)
  ensure
    post_flight
  end

  def pre_flight
    @buffers = []

    #audio
    @audio = Pathname.new(test_file)
    raise "Audio File: #{test_file} doesn't exist" unless @audio.exist?

    if path_to_orig_pt != 'none'
      raise "Could not find 'path_to_orig_pt': #{path_to_orig_pt}" unless Pathname.new(path_to_orig_pt).exist?
    end

    #presets
    @presets = []
    if factory_default
      preset = {}
      preset[:path] = FACTORY_DEFAULT
      preset[:basename] = FACTORY_DEFAULT
      preset[:filename] = FACTORY_DEFAULT
      @presets.push(preset)
    end

    if path_to_tfx != "none"
      preset_dir = Pathname.new(path_to_tfx)
      raise "Preset directory: #{path_to_tfx} doesn't exist" unless preset_dir.exist?
      preset_dir.children(true).each do |item|
        next if (item.extname != ".tfx")
        preset = {}
        preset[:path] = item
        preset[:basename] = item.basename
        preset[:filename] = "#{item.basename}".sub(item.extname, "")
        @presets.push(preset)
      end
    end

    raise "Preset list is an empty" if @presets.empty?
    "There are: #{@presets.size} preset(s)".log_status
  end

  def get_description(plugin)
    description = {}
    description[:type]    = plugin['type']

    description[:input_format] = plugin['input']
    description[:output_format] = plugin['output']

    description[:input]   = get_channels(plugin['input'])
    description[:output]  = get_channels(plugin['output'])

    match = /^(\d+)\.\s+(.+)\s+\[(.+)\]/.match(plugin['effect'])

    description[:idx]     = match[1]
    description[:name]    = match[2]
    description[:spec]    = match[3].gsub("\"", "").split(", ")

    return description
  end

  def get_channels(channel)
    number = case channel
    when 'mono'                                  then 1
    when 'stereo'                                then 2
    when 'lcr'                                   then 3
    when 'lcrs', 'quad', 'ambisonics(1st order)' then 4
    when 5.0                                     then 5
    when 5.1, 6.0                                then 6
    when 6.1, 7.0, '7.0DTS', '7.0SDDS'           then 7
    when 7.1, '7.1DTS', '7.1SDDS'                then 8
    when '7.0.2', 'ambisonics(2nd order)'        then 9
    when '7.1.2'                                 then 10
    when 'ambisonics(3rd order)'                 then 16
    else
       raise "Unknown stem-format: #{channel}"
    end

    return number
  end

  def process_signal(plugin, preset)
    @dsh.close
    @dsh = DigiShell.new(target)
    @dsh.load_dish("DAE")
    @dsh.load_dish("FFmt")

    @dsh.init_dae(sample_rate)
    @dsh.piproctrigger("manual") if plugin[:type] == BERLIN  # Setting piproctrigger to manual mode for synchronization with Native plug-ins

    input_buffers = case @audio.extname
    when WAV then @dsh.load_wav_file(@audio.to_s).values
    when BUF then @dsh.bfload(@audio.to_s).values
    else raise "Audio signal should have an extension #{WAV} or #{BUF}. Signal: #{@audio}"
    end

    input_buffers.sort!

    #copying buffer size
    output_buffers = []
    plugin[:output].times do
      buffer = @dsh.bclone(input_buffers[0]).values[0]
      output_buffers.push(buffer)
    end

    @dsh.run(plugin[:spec])

    #loading preset and setting default values
    if preset[:filename].to_s == FACTORY_DEFAULT
      @dsh.control.each {|ctrl| @dsh.control(ctrl['index'], ctrl['value_default'])}
    else
      begin
        @dsh.load_settings(preset[:path].to_s)
      rescue Exception => err
        "Attempt #{attempt+1}. Could not load preset: #{preset[:path]}\n#{err}".log_status
        raise err
      end
    end

    #processing signal
    @dsh.piproc(input_buffers, output_buffers)
    @dsh.unload

    result_buffers = []
    output_dir = target.build_folder + 'DTT'
    output_buffers.each.with_index do |buffer, idx|

      buffer_name = plugin[:spec].join("_").gsub(/\s+/, "") + "_" + "buffer#{idx+1}" + "_" + "#{Time.now.strftime("%b%d").to_i}"
      buffer_path = output_dir +  (buffer_name + BUF).to_s

      "#{buffer}: #{buffer_path}".log_verbose
      @dsh.bfsave(buffer, buffer_path.to_s, {"overwrite" => 1})
      result_buffers.push(buffer_path.to_s)
    end
    @dsh.close
    return result_buffers
  end

  def compare_buffers(buffer_list1, buffer_list2)
    if buffer_list1.size != buffer_list2.size
      message = "Plug-ins should have identical stem-format."
      message += " Output buffers for first plug-in: #{buffer_list1.size},"
      message += " for second plug-in: #{buffer_list2.size}"
      raise message
    end
    @dsh.close
    @dsh = DigiShell.new(target)

    results = []
    buffer_list1.each_index do |idx|
      b1 = load_buffer(buffer_list1[idx])
      b2 = load_buffer(buffer_list2[idx])
      result = @dsh.bacmp(b1,b2)["acmp_result"]
      result = CANCELLATION_MINIMUM if (result == "equal" || result == "inf")
      "Difference: #{result} dB\n\t#{buffer_list1[idx]}\n\t#{buffer_list2[idx]}".log_verbose
      results.push(result)
    end

    return results.max

  rescue Exception => err
    "ERROR: #{err}".log_status
  ensure
    @dsh.close
  end

  def load_buffer(buffer)
    buffer = buffer.to_s if buffer.instance_of?(Pathname)
    output = @dsh.bfload(buffer).values[0]
    return output
  end

  def remove_files(files)
    if (files.instance_of?(String) or files.instance_of?(Pathname))
      files = [files]
    end

    raise "ERROR: could not delete : #{files.inspect}" unless files.instance_of?(Array)

    files.each do |file|
      "Deleting file: #{file}".log_verbose
      file = Pathname.new(file) unless file.instance_of?(Pathname)
      begin
        file.rmtree if file.exist?
      rescue Exception => err
        "Could not delete file: #{file}".log_status
        "Reason: #{err}".log_verbose
        next
      end
    end
  end

  def post_flight
    @dsh.close if @dsh
    remove_files(@buffers)
  end
end
