#
#  DSH_AAXVAL_Load_Unload.rb
#  DTT
#
#  Created by Sergii Dekhtiarenko on 05/28/14.
#  Copyright 2014 by Avid Technology, Inc.
#

class DSH_AAXVAL_Load_Unload < Script

  def self.inputs
    return {
        :bundle_path    => ['DynamicsIII.aaxplugin'],
        :iteration_num  => [1000]
    }
  end

  def run
    if target.exist_on_target?(bundle_path)
      real_path = target.real_path(bundle_path)
    else
      return cancel("Could NOT find specified bundle: #{bundle_path}")
    end

    begin
      sub_result = self.create_sub_result
      sub_result.log_test_config({:iteration_num => iteration_num})

      dsh = DigiShell.new(target)
      dsh.enable_trace_facility(["DTF_AAXH_CLIENT", "DTP_LOWEST"])
      dsh.load_dish("aaxh") unless dsh.list_loaded_dishes.include?('aaxh')
      dsh.teardown
      dsh.init('debug_max')

      start_time = Time.now
      sub_result.start_time(start_time)

      iteration_num.times do |i|
        begin
          plugin_id = dsh.loadpi(real_path)['pluginID']
          dsh.unloadpi(plugin_id)
        rescue Exception => err
          raise "#{err} Iteration number: #{i+1}. Elapsed Time: #{Time.now - start_time}"
        end
      end

      end_time = Time.now
      elapsed_time = end_time - start_time
      sub_result.elapsed_time(elapsed_time)

      average_ms_per_iteration = elapsed_time * 1000/iteration_num
      sub_result.average_ms_per_iteration(average_ms_per_iteration)

      result_message = "Successfully loaded/unloaded plug-in #{iteration_num} times for #{elapsed_time} seconds"

      sub_result.add_log(result_message)

      dsh.close
      return pass(result_message)
    rescue Exception => err
      sub_result.add_log(err.message)
      err.backtrace.each{|l| l.log_verbose}
      dsh.close
      return fail(err)
    end
  end
end
