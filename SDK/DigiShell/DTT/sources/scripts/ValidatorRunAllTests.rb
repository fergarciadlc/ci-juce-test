# ValidatorRunAllTests.rb
#
# An example script for running AAX Validator tests
# via DTT and saving the results to a directory
# 
# Copyright 2015 by Avid Technology, Inc.
#
#

# Ruby core library modules are included via DishTestTool.rb (AAXTOOL-855)
#require 'fileutils'
#require 'tmpdir'

class ValidatorRunAllTests < Script

  def self.inputs
    return {
        :pi_path            => [''], #in order if you want to customise pi_path
        :result_format      => ['json',['xml', 'json', 'protobuf']],
        :out_path           => [Dir.tmpdir()],
        :mode               => ['all',['all', 'fast', 'required', 'info', 'tests']],
        :test_id            => [''] #optional if you want to run a specific test
    }
  end

  def run
    #
    # Set parameters
    #
    if pi_path.empty?
      if target.win?
        pi_path_fixed = 'C:\Program Files\Common Files\Avid\Audio\Plug-Ins'
      else
        pi_path_fixed = '/Library/Application Support/Avid/Audio/Plug-Ins'
      end
    else
      pi_path_fixed = pi_path
    end
    
    #
    # Validate parameters
    #
    return fail("Could not find path: #{pi_path}") unless Pathname(pi_path_fixed).exist?
    return fail("Could not find path: #{out_path}") unless Pathname(out_path).exist?

    #
    # Initialize
    #
    "Initializing DSH".log_status
    dsh = DigiShell.new(target)
    dishes = dsh.list_loadable_dishes
	return fail("Could not find 'aaxval' dish") unless dishes.include?('aaxval')
    dsh.load_dish("aaxval")
    
    #
    # Log the list of plug-ins that will be tested
    #
    "Plug-ins to test:".log_status
    plugins = dsh.findaaxplugins(pi_path_fixed)
    plugins['aaxplugin_paths'].each do |aaxplugin_path|
      "  #{aaxplugin_path}".log_status
    end
    
    #
    # List the tests that will be run
    #
    listtests_args = {'stringformat' => 'yaml', 'detail' => 'min'}
    case mode
    when 'all'
      # no additional filtering required
    when 'fast'
      listtests_args['mod'] = 'mod_tests_fast'
    when 'required'
      listtests_args['coll'] = 'col_required'
    when 'info'
      listtests_args['coll'] = 'col_info'
    when 'tests'
      listtests_args['coll'] = 'col_tests'
    else
      return fail("Encountered unknown mode: #{mode}")
    end
    
    if !test_id.empty?
      listtests_args['test'] = test_id
    end
    
    "Tests to run:".log_status
    tests = dsh.listtests(listtests_args)
    tests['test_info'].each do |test_info|
      "  #{test_info['id']}".log_status
    end 
    
    #
    # Create the output folder if it does not exist
    #
    out_dir = "#{out_path}/run_all_tests_result"
    FileUtils.mkdir_p out_dir
    
    #
    # Run all tests
    #
    "Running tests".log_status
    test_cfgs_for_result_status = Hash.new { |h,k| h[k] = Array.new } # Create an empty Array for any unknown key
    
    # We could simply execute "dsh.runtests" on the target path, but instead
    # we run each test individually for better logging in the case of a failure
    # In the future the AAXValidator framework should provide a callback for
    # obtaining test results on the fly as individual tests complete.
    #
    # For reference, here is the quick-and-dirty version:
    #results = dsh.runtests(pi_path_fixed)
    #results['results'].each do |result_info|
    #  result_ref = result_info['result_ref']
    #  result_path = "#{out_dir}/#{result_info['id']}__#{result_info['result']['connection_id']}.#{result_format}"
    #  dsh.saveresult({'result_ref' => result_ref, 'result_path' => result_path, 'stringformat' => result_format})
    #  "Saved result file to: #{result_path}".log_status
    #end
    
    tests['test_info'].each do |test_info|
      test_id = test_info['id']
      plugins['aaxplugin_paths'].each do |aaxplugin_path|
        test_cfg = "test_id: #{test_id}, path: #{aaxplugin_path}"
        "  #{test_cfg}".log_status
        result = dsh.runtest({'test' => test_info['id'], 'path' => aaxplugin_path, 'detail' => 'all', 'stringformat' => 'yaml', 'runmode' => 'serial'})
        
        # Iterate through each result - there is only one in this case, since
        # we used 'runtest', not 'runtests' - but the same logic can be used
        # if 'runtests' is swapped in.
        result['results'].each do |result_info|
          # Check and aggregate the test results, and also log the result
          result_status = result_info.key?('result_status') ? result_info['result_status'] : "(not provided)"
          test_cfgs_for_result_status[result_status].push(test_cfg)
          "    result_status: #{result_status}".log_status
          
          # Save the result to a file on disk
          result_ref = result['result_ref']
          result_path = "#{out_dir}/#{result['id']}__#{result_info['connection_id']}.#{result_format}"
          dsh.saveresult({'result_ref' => result_ref, 'result_path' => result_path, 'stringformat' => result_format})
          "    Saved result file to: #{result_path}".log_status
        end
      end
    end 
    
    #
    # Tear down
    #
    "Tearing down DSH...".log_status
    dsh.close
    "done".log_status
    
    #
    # Log the results and set pass/fail status
    #
    "Test results:".log_status
    test_cfgs_for_result_status.each do |key, value|
      "  #{value.length} tests returned #{key}:".log_status
      "    #{value.join("\n    ")}".log_status
    end
    
    all_tests_passed = true
    for cur_failure_key in ['E_COMPLETED_FAIL', 'E_LOST', 'E_TIMEOUT'] # These are the results that we consider as failures
      all_tests_passed = all_tests_passed && !test_cfgs_for_result_status.key?(cur_failure_key)
    end
    
    result = SuiteResult::RESULTSTATUS_UNKNOWN
    if all_tests_passed
      result = pass("All tests passed, were canceled, or did not provide results")
    else
      result = fail("At least one test failed or was lost")
    end
    
    return result
    
  rescue Exception => e
    dsh.close unless dsh.nil?
    e.backtrace.each { |line| line.log_status }
    return fail(e.message)
  end
end
