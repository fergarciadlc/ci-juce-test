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
      :original_plugin    => [["Digi", "dn3d", "DMRT"]],
      :shipping_files     => ["none"],
      :plugin_to_test     => [["Digi", "dn3d", "d3mm"]],
      :aax_bundle_name    => ["none",["pi_name.aaxplug"]],
      :sample_rate        => [44100,[44100,48000,88200,96000,176400,192000]],
      :test_file          => ['Audio file in .wav format with full path'],
      :path_to_tfx        => ['none'],
      :threshold          => [-96],
      :path_to_orig_pt    => ["none"],
      :default_test       => [true,[true,false]],
      :save_worse_results => [false, [false, true]],

      :slave              => [false, [false, true]],      #configured by the script. Don't change
      :preset_path        => ["full path to preset"]      #configured by the script. Don't change
    }
  end

  def self.gui_order
    return [:original_plugin,:shipping_files,:plugin_to_test,:sample_rate,:test_file,:path_to_tfx,:threshold, :save_worse_results, :slave, :preset_path]
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

    if slave
      #run_test_as_slave
      return pass(run_test_as_slave.inspect)
    end

    @dsh = DigiShell.new(target)
    @dsh.load_dish("DAE")
    plugins = @dsh.run
    @dsh.close

    legacy_plugin = {}
    target_plugin = {}

    legacy_plugin_found = false
    target_plugin_found = false

    plugins.each do |plugin|
      if plugin['effect']["#{original_plugin.inspect}"]
        legacy_plugin = get_description(plugin)
        legacy_plugin_found = true
      end

      if plugin['effect']["#{plugin_to_test.inspect}"]
        target_plugin = get_description(plugin)
        target_plugin_found = true
      end
    end

    raise "Could not find specified plugin: #{original_plugin.inspect}" unless legacy_plugin_found
    raise "Could not find specified plugin: #{plugin_to_test.inspect}" unless target_plugin_found
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

        if @use_orig_pt
          buffers_target_plugin = process_signal(target_plugin, preset)
          @buffers += buffers_target_plugin

          process_signal_as_slave(legacy_plugin, preset)

          buffers_dir = Pathname.new(path_to_orig_pt) + 'DTT'
          buffer_base_name = legacy_plugin[:spec].join("_").gsub(/\s+/, "")

          legacy_plugin[:output].times do |index|
            buffer_name = buffer_base_name + "_" + "buffer#{index+1}" + "_" + Time.now.strftime("%b%d") + BUF
            buffer_path = buffers_dir + buffer_name
            buffers_legacy_plugin.push(buffer_path.realpath.to_s)
          end
          @buffers += buffers_legacy_plugin
        else #if path_to_orig_pt
          buffers_target_plugin = process_signal(target_plugin, preset)
          @buffers += buffers_target_plugin

          if @move_plugin
            FileUtils.move(@plugins_dir + aax_bundle_name, @unused_plugins_dir)
            FileUtils.cp_r(@src_plugin_files, @plugins_dir)
          end

          buffers_legacy_plugin = process_signal(legacy_plugin, preset)
          @buffers += buffers_legacy_plugin

          if @move_plugin
            files_to_remove = @src_plugin_files.map {|file| @plugins_dir + file.basename}
            FileUtils.rm_r(files_to_remove)
            FileUtils.move(@unused_plugins_dir + aax_bundle_name, @plugins_dir)
          end
        end #if @use_orig_pt

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
    @build_dir = target.get_test_tool_folder
    @buffers = []
    @move_plugin = false
    @use_orig_pt = false

    #audio
    @audio = Pathname.new(test_file)
    raise "Audio File: #{test_file} doesn't exist" unless @audio.exist?

    if path_to_orig_pt != "none" && !slave
      @use_orig_pt = true
      @runsuite_path = DishTestTool::BIN_DIR + 'runsuite.rb'
      launch_dtt_monitor
    end

    return if slave

    if target.mac?
      @plugins_dir = @build_dir + "Plug-Ins"
      @unused_plugins_dir = @build_dir + "Plug-Ins (Unused)"
    else
      @plugins_dir = @build_dir.parent + "dae" + "plug-ins"
      @unused_plugins_dir = @build_dir.parent + "dae" + "plug-ins (unused)"
    end

    #worse results
    @worse_outputs_dir =  @build_dir + 'DTT' + 'worseoutputs'
    @worse_outputs_dir.mkpath if(!@worse_outputs_dir.exist? && save_worse_results)
    @worse_results = {}

    if ((shipping_files != "none") && (shipping_files != nil))
      @move_plugin = true
      @src_plugin_files = shipping_files.split(/,\s*/) if shipping_files.instance_of?(String)
      raise "shipping_files should be an Array or string" if !shipping_files.instance_of?(Array)

      @src_plugin_files.map! {|file| Pathname.new(file)}
      @src_plugin_files.each {|file| raise "Could not find file: #{file}" unless file.exist?}
      "Will be used original plugin:".log_status
      @src_plugin_files.each{|file| "\t#{file.basename}".log_status}
    end

    #presets
    @presets = []
    if default_test
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
    when 'mono'                         then 1
    when 'stereo'                       then 2
    when 'lcr'                          then 3
    when 'lcrs', 'quad'                 then 4
    when 5.0                            then 5
    when 5.1, 6.0                       then 6
    when 6.1, 7.0, '7.0DTS', '7.0SDDS'  then 7
    when 7.1, '7.1DTS', '7.1SDDS'       then 8
    else
       raise "Unknown stem-format: #{channel}"
    end

    return number
  end

  def process_signal(plugin, preset)
    @dsh.close
    @dsh = DigiShell.new(target)

    attempt = 0
    begin
      @dsh.load_dish("DAE")
    rescue  Exception => err
      "attempt #{attempt+1}. Could not load DAE dish: #{err}".log_status
      retry if (attempt +=1) <= MAX_ATTEMPTS
      raise "Could not load DAE dish: #{err}"
    end

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

    sleep 1.5

    #processing signal
    @dsh.piproc(input_buffers, output_buffers)
    @dsh.unload

    result_buffers = []
    output_dir = @build_dir + 'DTT'
    output_buffers.each.with_index do |buffer, idx|
      if(save_worse_results && (plugin[:type] == BERLIN))
        worse_result_name = preset[:filename] + "_" + @audio.basename.to_s.sub(@audio.extname, "") + WAV
        worse_result_path = @worse_outputs_dir + worse_result_name.downcase
        @worse_results[preset[:filename].to_sym] = {:path => worse_result_path, :failed => false}
        @dsh.save_wav_file(buffer, worse_result_path.to_s)
      end

      buffer_name = plugin[:spec].join("_").gsub(/\s+/, "") + "_" + "buffer#{idx+1}" + "_" + Time.now.strftime("%b%d")
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
    attempt = 0
    begin
      output = @dsh.bfload(buffer).values[0]
    rescue Exception => err
      "attempt: #{attempt+1}. #{err}".log_status
      sleep 2
      retry if (attempt += 1) <= MAX_ATTEMPTS
      raise "ERROR: #{err}"
    end
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

  def launch_dtt_monitor
    dtt_monitor = (Pathname(path_to_orig_pt) + 'DTT' + 'sources' + 'bin' + 'run_monitor.rb').realpath

    raise "Could not find file: #{dtt_monitor}" unless dtt_monitor.exist?

    if target.mac?
      launch_dtt_monitor_cmd = "osascript -e \'tell app \"Terminal\" to do script \"#{RUBY} #{dtt_monitor}\"\'"
    else
      launch_dtt_monitor_cmd = "start \"DTT Monitor\" #{RUBY} #{dtt_monitor}"
    end
    "Launching DTT Monitor...".log_verbose
    system(launch_dtt_monitor_cmd)
  end

  def quit_dtt_monitor
    quit_dtt_monitor_cmd = "#{RUBY} #{@runsuite_path} -t #{LOCAL_HOST} QuitGST"
    "Quitting DTT Monitor...".log_verbose
    system(quit_dtt_monitor_cmd)
  end

  def process_signal_as_slave(plugin, preset)
    run_suite_cmd = "#{RUBY} #{@runsuite_path}"
    run_suite_cmd += " DSH_SigCancellation -x 10"
    run_suite_cmd += " -t #{LOCAL_HOST}"
    run_suite_cmd += " -a original_plugin=\"#{plugin[:spec].join(",")}\""
    run_suite_cmd += " -a slave=true"
    run_suite_cmd += " -a test_file=\"#{@audio}\""
    run_suite_cmd += " -a preset_path=\"#{preset[:path]}\""
    run_suite_cmd += " -a sample_rate=#{sample_rate}"

    system(run_suite_cmd)
  end

  def run_test_as_slave
    begin
      preset = {}
      if preset_path == FACTORY_DEFAULT
        preset[:path] = FACTORY_DEFAULT
        preset[:basename] = FACTORY_DEFAULT
        preset[:filename] = FACTORY_DEFAULT
      else
        preset[:path] = Pathname.new(preset_path)
        preset[:basename] = preset[:path].basename
        preset[:filename] = "#{preset[:path].basename}".sub( preset[:path].extname, "")
      end #if

      @dsh = DigiShell.new(target)
      @dsh.load_dish("DAE")
      plugins = @dsh.run
      legacy_plugin = {}
      plugins.each do |plugin|
        if plugin['effect']["#{original_plugin.inspect}"]
          legacy_plugin = get_description(plugin)
          break
        end #if
      end #do
      @dsh.close
      raise "Could not find plug-in with spec: #{original_plugin.inspect}. Make Sure that plug-in exist for current sample rate " if legacy_plugin.empty?
      outputs = process_signal(legacy_plugin, preset).collect{|b| Pathname.new(b).basename.to_s}
      return outputs.join("\n")
    rescue Exception => err
      err.backtrace.each {|line| line.log_verbose}
      raise err
    end
  end

  def post_flight
    @dsh.close if @dsh
    remove_files(@buffers)
    quit_dtt_monitor if (@use_orig_pt && !slave)

    #restore plug-ins (remove RTAS if exist; copy AAX back)
    if @move_plugin
      files_to_remove = @src_plugin_files.map {|file| @plugins_dir + file.basename}
      files_to_remove.each {|file| FileUtils.rm_r(file) if file.exist?}
      FileUtils.move(@unused_plugins_dir + aax_bundle_name, @plugins_dir) unless (@plugins_dir + aax_bundle_name).exist?
    end
  end
end
