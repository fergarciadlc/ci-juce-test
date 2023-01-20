#
#  DSH_AAXVAL_Support_General.rb
#  DTT
#
#  Created by Sergii Dekhtiarenko on 06/24/14.
#  Copyright 2014 by Avid Technology, Inc.
#

class DSH_AAXVAL_Support_General < Script

  def self.inputs
    return {
        :bundle_path => ['Trim.aaxplugin']
    }
  end

  def run
    if target.exist_on_target?(bundle_path)
      real_path = target.real_path(bundle_path)
    else
      return fail("Could NOT find specified bundle: #{bundle_path}")
    end

    dsh = DigiShell.new(target)
    dsh.enable_trace_facility(['DTF_AAXH_CLIENT', 'DTP_LOWEST'])
    dsh.load_dish('aaxh')
    dsh.init('debug_max')

    plugin_id = dsh.loadpi(real_path)['pluginID']

    effects = dsh.listeffects(plugin_id)['effectIDs']

    effects.each do |effect|
      sub_result = self.create_sub_result
      sub_result.effect_id = effect['effectID']

      begin
        info = dsh.grab_effect_info(plugin_id, effect['effectIdx'])  
      rescue Exception => e
        sub_result.add_log("ERROR: " + e.message)
        raise e
      end
      
      sub_result.log_tree(info)
    end

    dsh.teardown
    dsh.close

    return pass('See plug-in description')
  rescue Exception => err
    err.backtrace.each{|s| s.log_status}
    dsh.close
    return fail(err.message)
  end
end
