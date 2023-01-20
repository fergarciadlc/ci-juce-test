# ValidatorRunAllTests.rb
#
# An example script for running AAX Validator tests
# via DTT and saving the results to a directory
# 
# Copyright 2015 by Avid Technology, Inc.
#
#

class ValidatorRunAllTests < Script

  def self.inputs
    return {
        :pi_path            => ['/Library/Application Support/Avid/Audio/Plug-Ins'],
        :result_format      => ['json',['xml', 'json', 'yaml', 'protobuf']],
        :out_path           => ['/private/tmp']
    }
  end

  def self.gui_order
    return [:pi_path, :result_format, :out_path]
  end

  def run
    # Validate parameters
    raise "Could not find path: #{pi_path}" unless Pathname(pi_path).exist?
    raise "Could not find path: #{out_path}" unless Pathname(out_path).exist?

    # Initialize
    dsh = DigiShell.new(target)
    dsh.load_dish("aaxval")

    # Run all tests
    results = dsh.runtests(pi_path)
    
    # Create the output container dir if it does not exist
    out_dir = "#{out_path}/run_all_tests_result"
    FileUtils.mkdir_p out_dir
    
    # Save each result to a separate file on disk
    results['results'].each do |result_info|
      result_ref = result_info['result_ref']
      result_path = "#{out_dir}/#{result_info['id']}__#{result_info['result']['connection_id']}.#{result_format}"
      dsh.saveresult({'result_ref' => result_ref, 'result_path' => result_path, 'stringformat' => result_format})
      "Saved result file to: #{result_path}".log_status
    end
    
    # Tear down
    dsh.close
    
    return pass
    
  rescue Exception => e
    dsh.close unless dsh.nil?
    e.backtrace.each { |line| line.log_status }
    return fail(e.message)
  end
end
