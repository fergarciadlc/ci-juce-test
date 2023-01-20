#
#  ZetaDish.rb
#
#  Created by Dmytro Pravdiuk in November 2021.
#  Copyright 2021 by Avid Technology, Inc.

module DishTestTool

=begin rdoc
Class for interacting with Zeta dish
=end
  class ZetaDish
    CC_ASYNC_DELAY = 0.25 # 250 milliseconds
    
    def initialize(dsh)
      print("Loading Zeta dish... ")
      @dsh = dsh
      @dsh.load_dish(["Zeta"])
      puts "done"
    end

    def init_device(id)
      @dsh.init_zeta(id)
      sleep(CC_ASYNC_DELAY)
    end

    def uninit_device(id)
      res = @dsh.teardown_zeta(id)
    end

    def enable_sample_rate_lock(id, lock_state_value)
      args = {
        'device_idx' => id,
        'lock_state' => lock_state_value
      }
      res = @dsh.enable_sample_rate_lock(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def execute_command(id, command)
      args = {
        'device_idx' => id,
        'command' => command
      }
      res = @dsh.execute_command(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def execute_shell_command(id, cmd, timeout_value = nil, shell_type = nil)
      args = {
        'device_idx' => id,
        'command' => cmd,
      }

      if (shell_type.nil? == false)
        args.merge!({"shell" => shell_type})
      end

      if (timeout_value.nil? == false)
        args.merge!({"timeout" => timeout_value})
      end

      res = @dsh.execute_shell_command(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_power(id, power_state_value)
      args = {
        'device_idx' => id,
        'power_state' => power_state_value
      }
      res = @dsh.set_power(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def flash_fw(id, path_value, need_reboot = -1, commit_value = -1)
      args = {
        'device_idx' => id,
        'path' => path_value
      }

      if need_reboot != -1
        args.merge!({"reboot" => need_reboot})
      end

      if commit_value != -1
        args.merge!({"commit" => commit_value})
      end

      res = @dsh.flash_fw(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def get_fw_ver(id)
      res = @dsh.get_fw_ver(id)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def get_params(id, list_of_param_types=nil)
      args = {
        'device_idx' => id
      }
      args.merge!({'param_types' => list_of_param_types}) if not list_of_param_types.nil?
      res = @dsh.get_params(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def get_temperature(id)
      res = @dsh.get_temperature(id)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_defaults(id)
      res = @dsh.set_defaults(id)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_dim_amount(id, dim_amount_value)
      args = {
        'device_idx' => id,
        'dim_amount' => dim_amount_value
      }
      res = @dsh.set_dim_amount(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_headphone_source(id, hp_args)
      args = {
        'device_idx' => id
      }
      args.merge!(hp_args)

      res = @dsh.set_headphone_source(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_input_gain_bypass(id, inputs, bypass_value)
      args = {
        'device_idx' => id,
        'physical_inputs' => inputs,
        'bypass' => bypass_value
      }

      res = @dsh.set_input_gain_bypass(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_monitors(id, monitor_type_values, do_enable_value)
      args = {
        'device_idx' => id,
        'monitor_types' => monitor_type_values,
        'do_enable' => do_enable_value
      }
      res = @dsh.set_monitors(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_opticalIO(id, val_selector, port_value, io_selector)
      args = {
        'device_idx' => id,
        'val' => val_selector,
        'port' => port_value,
        'io' => io_selector
      }
      res = @dsh.set_opticalIO(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_output_reference_level(id, outputs, level_value)
      args = {
        'device_idx' => id,
        'physical_outputs' => outputs,
        'level' => level_value
      }
      res = @dsh.set_output_reference_level(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_preamp_bypass(id, list_of_inputs, preamp_bypass_value)
      args = {
        'device_idx' => id,
        'pre_mic_inputs' => list_of_inputs,
        'preamp_bypass' => preamp_bypass_value
      }
      res = @dsh.set_preamp_bypass(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_preamp_input_gain(id, list_of_inputs, preamp_input_gain_value)
      args = {
        'device_idx' => id,
        'pre_mic_inputs' => list_of_inputs,
        'preamp_input_gain' => preamp_input_gain_value
      }
      res = @dsh.set_preamp_input_gain(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_preamp_input_source(id, list_of_inputs, preamp_input_source_value)
      args = {
        'device_idx' => id,
        'pre_mic_inputs' => list_of_inputs,
        'preamp_input_source' => preamp_input_source_value
      }
      res = @dsh.set_preamp_input_source(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_preamp_input_z(id, list_of_inputs, preamp_input_z_value)
      args = {
        'device_idx' => id,
        'pre_mic_inputs' => list_of_inputs,
        'preamp_input_z' => preamp_input_z_value
      }
      res = @dsh.set_preamp_input_z(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_preamp_link(id, list_of_inputs, preamp_link_value)
      args = {
        'device_idx' => id,
        'pre_mic_inputs' => list_of_inputs,
        'preamp_link' => preamp_link_value
      }
      res = @dsh.set_preamp_link(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_preamp_phantom(id, list_of_inputs, preamp_phantom_value)
      args = {
        'device_idx' => id,
        'pre_mic_inputs' => list_of_inputs,
        'preamp_phantom' => preamp_phantom_value
      }
      res = @dsh.set_preamp_phantom(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_preamp_phase(id, list_of_inputs, preamp_phase_value)
      args = {
        'device_idx' => id,
        'pre_mic_inputs' => list_of_inputs,
        'preamp_phase' => preamp_phase_value
      }
      res = @dsh.set_preamp_phase(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def set_tb_mic_gain(id, talkback_mic_gain_value)
      args = {
        'device_idx' => id,
        'talkback_mic_gain' => talkback_mic_gain_value
      }
      res = @dsh.set_tb_mic_gain(args)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def supported_by_both(id)
      res = @dsh.supported_by_both(id)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def supported_by_client(id)
      res = @dsh.supported_by_client(id)
      sleep(CC_ASYNC_DELAY)
      res
    end

    def supported_by_service(id)
      res = @dsh.supported_by_service(id)
      sleep(CC_ASYNC_DELAY)
      res
    end

    # UTILS
    def shell_command(id, cmd, print_cmd = false)

      puts cmd if print_cmd
      raise("Incorrect shell command.") if !cmd.is_a? String

      res = execute_shell_command(id, cmd)
      raise ("Failed to initialize. Please reboot while holding B button.") if res['stderr'] == "denied"
      raise ("Command #{cmd} execution failed due to: #{res['stderr']}") if res['exit_code'].to_i != 0

      return res['stdout']

    end
  end

end # module
