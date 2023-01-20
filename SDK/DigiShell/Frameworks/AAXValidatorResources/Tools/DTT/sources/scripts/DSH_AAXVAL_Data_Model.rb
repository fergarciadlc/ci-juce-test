#
#  DSH_AAXVAL_Data_Model.rb
#  DTT
#
#  Created by Sergii Dekhtiarenko on 03/25/14.
#  Copyright 2014 by Avid Technology, Inc.
#

class DSH_AAXVAL_Data_Model < Script

  def self.inputs
    return {
        :bundle_path => ['LoadUnloadFail.aaxplugin']
    }
  end

  def run
    if target.exist_on_target?(bundle_path)
      real_path = target.real_path(bundle_path)
    else
      return fail("Could NOT find specified bundle: #{bundle_path}")
    end

    begin
      dsh = DigiShell.new(target)
      failures = []

      dsh.enable_trace_facility(["DTF_AAXH_CLIENT", "DTP_LOWEST"])
      dsh.load_dish("aaxh") unless dsh.list_loaded_dishes.include?('aaxh')
      dsh.teardown
      dsh.init('debug_max')

      plugin_id = dsh.loadpi(bundle_path)['pluginID']
      effects = dsh.listeffects(plugin_id)['effectIDs']

      effects.each do |effect|
        effect_failures = []
        sub_result = self.create_sub_result
        effect_id = effect['effectID']
        sub_result.effect_id = effect_id
        sub_result.start_time

        effect_idx = effect['effectIdx']
        AAXHDishUtils::PLATFORM.each do |pl_key, pl_val|
          AAXHDishUtils::SAMPLE_RATES.each do |sr_key, sr_val|
            AAXHDishUtils::STEM_FORMAT.each do |in_sf_key, in_sf_val|
              AAXHDishUtils::STEM_FORMAT.each do |out_sf_key, out_sf_val|
                params = {'plugin' => plugin_id, 'effect' => effect_idx, 'plat' => pl_val,
                          'in' => in_sf_val, 'out' => out_sf_val, 'rate' => sr_val, 'alg' => false}

                if dsh.geteffectsupportscontext(params)['supportsHostContext']
                  begin
                    log_info = params.map{|k,v| "#{k}: #{v}"}.join(", ")
                    "#{log_info}".log_verbose

                    inistance = dsh.instantiateforcontext(params)

                    instance_id = inistance['instanceID']
                    plugin_id = params['plugin']

                    dsh.uninstantiate({'plugin' => plugin_id, 'inst' => instance_id})

                  rescue Exception => err
                    effect_failures.push({:err => err.message, :params => params})
                    next
                  end
                end
              end
            end
          end
        end

        if effect_failures.empty?
          sub_result.add_log("Successsfully created data models for: #{effect_id}")
        else
          failures += effect_failures
          effect_failures.each {|err| sub_result.add_log('ERROR:'+ err[:err] + " " +  err[:params].map{|k,v| "#{k}: #{v}"}.join(', '))}
        end

        sub_result.elapsed_time
      end

      msg = "#{bundle_path.match(/[^\\\/]+$/)} Successfully created Data Model for all effects!"

      unless failures.empty?
        if (failures.size == 1) then
          msg = "Encountered error: #{failures[0][:err]}"
        else
          msg = "Encountered #{failures.size} errors. See verbose log for details"
        end

        failures.each do |elem|
          verbose_str = elem[:err] + " " +  elem[:params].map{|k,v| "#{k}: #{v}"}.join(', ')
          verbose_str.log_verbose
        end
      end

      dsh.close
      return failures.empty? ? pass(msg) : fail(msg)

    rescue Exception => err
      dsh.close
      return fail(err)
    end
  end
end
