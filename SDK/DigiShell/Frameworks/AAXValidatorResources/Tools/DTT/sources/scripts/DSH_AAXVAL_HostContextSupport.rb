#
#  DSH_AAXVAL_HostContextSupport.rb
#  Validator
#
#  Created by Sergii Dekhtiarenko on 11/05/14.
#  Copyright 2014 by Avid Technology, Inc.
#

class DSH_AAXVAL_HostContextSupport < Script

  def self.inputs
    return {
        :bundle_path => ['LoadUnloadFail.aaxplugin'],
    }
  end

  def run
    if target.exist_on_target?(bundle_path)
      real_path = target.real_path(bundle_path)
    else
      return fail("Could NOT find specified bundle: #{bundle_path}")
    end

    begin
      test_fail = false

      dsh = DigiShell.new(target)
      info_items = ['plat', 'in', 'out', 'rate', 'alg']

      dsh.enable_trace_facility(["DTF_AAXH_CLIENT", "DTP_LOWEST"])
      dsh.load_dish("aaxh") unless dsh.list_loaded_dishes.include?('aaxh')
      dsh.teardown
      dsh.init('debug_max')

      plugin_id = dsh.loadpi(real_path)['pluginID']
      effects = dsh.listeffects(plugin_id)['effectIDs']

      effects.each do |effect|
        valid_combinations = 0
        sub_result = self.create_sub_result
        effect_id = effect['effectID']

        sub_result.effect_id = effect_id
        sub_result.start_time

        effect_idx = effect['effectIdx']
        AAXHDishUtils::PLATFORM.each do |pl_key, pl_val|
          AAXHDishUtils::SAMPLE_RATES.each do |sr_key, sr_val|
            AAXHDishUtils::STEM_FORMAT.each do |in_sf_key, in_sf_val|
              AAXHDishUtils::STEM_FORMAT.each do |out_sf_key, out_sf_val|
                host_context = {'plugin' => plugin_id, 'effect' => effect_idx, 'plat' => pl_val,
                          'in' => in_sf_val, 'out' => out_sf_val, 'rate' => sr_val, 'alg' => false}

                supported = dsh.geteffectsupportscontext(host_context)['supportsHostContext']


                info = Hash.new
                info['host_context_support'] = Hash.new

                host_context.select!{|k,v| info_items.include?(k)}
                host_context_str = host_context.map{|k, v| "#{k}: #{v}"}.join(', ')

                info['host_context_support']['host_context'] = host_context_str
                info['host_context_support']['supported'] = supported

                valid_combinations += 1 if supported

                sub_result.log_tree(info)
              end
            end
          end
        end
        sub_result.elapsed_time
        if valid_combinations == 0
          sub_result.add_log("ERROR: #{effect_id} doesn't have any valid combination")
          test_fail = true
        end
      end

      msg = "#{bundle_path.match(/[^\\\/]+$/)}"

      dsh.close
      return test_fail ? fail(msg) : pass(msg)

    rescue Exception => err
      dsh.close
      return fail(err.message)
    end
  end
end
