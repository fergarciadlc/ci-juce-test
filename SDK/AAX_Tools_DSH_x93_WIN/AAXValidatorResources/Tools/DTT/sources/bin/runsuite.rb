#  == Synopsis
#  runsuite.rb -- Runs
#
#  == Usage
#
#  runsuite [suite_or_script] [options]
#    -a, --arg KEY=VAL
#    -d, --debug_mode
#    -e, --no_log_elapsed_time
#    -f, --file FILE
#    -h, --help
#    -o, --no_move_options
#    -p, --no_pref_delete
#    -l, --list
#    -t, --target ADDR
#    -v, --verbose
#    -x, --timeout_factor VAL
#
#
#  -a, --arg KEY=VAL: 
#  Run scripts with argument KEY equal to VAL. As
#  many arguments may be set as desired.
#
#  -d, --debug_mode:
#  By default, runsuite traps errors and attempts to continue to
#  the next script in a suite, cleaning up the target by quitting
#  Pro Tools. When this flag is set, runsuite exits on any error,
#  leaving the target state unchanged for inspection.
#
#  -e, --no_log_elapsed_time
#  By default, runsuite prints the elapsed time before every line
#  of the log. When this flag is set, it doesn't.
#
#  -f, --file FILE
#  Console output will be logged to FILE instead of STDERR. Note: regular
#  set of log files will not be affected.
#
#  -h, --help:
#  Shows this help info.
#
#  -n, --notify ADDRS:
#  Mails suite log to (comma-separated) ADDRS.
#
#  -o, --no_move_options
#  By default, runsuite backs up any DigiOptions file it finds,
#  writes its own establishing a known state and preventing
#  Software Updater launch, and restores it at the end of the suite.
#  This option skips that process and runs with current settings.
#
#  -p, --no_pref_delete:
#  By default, runsuite deletes Pro Tools preferences on the target
#  machine before running the suite, so that all suite start from
#  the same state. If this flag is set, the current preferences are
#  used.
#
#  -s, --simplified_output
#  Pares down the console output to the bare minimum while running:
#  which test is running, and whether it passed or not.
#
#  -t, --target ADDR:
#  Specifies the target machine on which test is to be run. Defaults
#  to localhost. gst must be running on the target machine.
#
#  -v, --verbose:
#  By default, the console mirrors the standard log file. If this
#  flag is set, the console mirrors the verbose log file, which
#  includes a record of each transaction between runsuite and the
#  target.
#
#  -x, --timeout_factor VAL:
#  Multiplies all communication timeout values by VAL. Useful when 
#  running tests with large performance loads.
#
#  -l, --list:
#  List of available scripts with number equivalent
#
#  == Author
#  Tim Walters, Digidesign
#
#  == Copyright
#  Copyright 2014 by Avid Technology, Inc.

require 'pathname'
require 'optparse'
require 'rbconfig'

$:.push(Pathname.new($0).parent.parent.realpath.to_s)

require 'DishTestTool'

def warn(str)
  str.abort
end  

def scripts_hash
  gss_suites = SUITE_DIR.children(false).select {|suite| suite.extname == ".gss"}
  suites = gss_suites.collect {|suite| suite.to_s.sub(suite.extname, "")}

  script_number = {}
  scripts  = suites + TEST_SCRIPTS.to_a.collect{|script| script.to_s}.sort
  scripts.each_with_index do |script, number|
    script_number[number] = script
  end
  return script_number
end

def show_scripts_hash
  scripts_hash.keys.sort.each{|key| puts "#{key} => #{scripts_hash[key]} " }
end

delete_prefs = true
disable_digitrace = false
debug_mode = false
target_addr = nil
script = nil
args = {}
verbose = false
log_elapsed_time = true
move_options = true
custom_log_file = nil
log_dir = nil
simplified_output = false

opts = OptionParser.new
opts.on('--script SCRIPT') {|s| script = s}
opts.on('-f', '--file FILE') { |str| custom_log_file = Pathname.new(str) }
opts.on('-g', '--logdir LOGDIR') { |str| log_dir = Pathname.new(str) }
opts.on('-h', '--help') { puts opts; exit }
opts.on('-w', '--disable_digitrace') {disable_digitrace = true}
opts.on('-p', '--no_pref_delete') { delete_prefs = false }
opts.on('-d', '--debug') { debug_mode = true }
opts.on('-t', '--target ADDR') { |addr| target_addr = addr }
opts.on('-s', '--simplified_output') { simplified_output = true }
opts.on('-l', '--list') { show_scripts_hash; exit }
opts.on('-a', '--arg KEY=VAL') do |str|
  key, val = str.split('=')
  key = key.downcase.to_sym

  if args[key]
	  args[key] = [args[key]] unless args[key].instance_of?(Array)
	  args[key].push(val)
  else
		args[key] = val
  end
end

opts.on('-x', '--timeout_factor VAL') do |str|
  begin
    TEST_INFO.timeout_factor = Float(str)
  rescue
    "Invalid timeout factor: #{str}.".abort(self)
  end
end

opts.on('-v', '--verbose') { verbose = true }
opts.on('-e', '--no_log_elapsed_time') { log_elapsed_time = false }
opts.on('-o', '--no_move_options') { move_options = false }
remainder = opts.parse(ARGV)

if remainder.empty? and script.nil?
  puts opts
  exit
elsif !remainder.empty? and !script.nil?
	puts "You coud NOT specify several scripts to execute. Plase use SUITEs insted"
	exit
elsif remainder.empty? and !script.nil?
	remainder.push(script)
end

# Nil OK here
script_or_suite = remainder
if script_or_suite[0] =~ /^\d+$/
  script_number = script_or_suite[0].to_i
  script_or_suite[0] = scripts_hash[script_number]
end

# if there's only one script/suite argument, check to see if it refers to
# a suite spec in the suites folder
if (script_or_suite and script_or_suite.size == 1)
	suitepath = SUITE_DIR + (script_or_suite[0] + ".gss")
	if (suitepath.exist?)
		suite = YAML::load(IO.read(suitepath))
		suitesettings = suite['suitesettings']

		verbose = suitesettings['verbose'] if suitesettings.key?('verbose')
		delete_prefs = !suitesettings['retainPrefs'] if suitesettings.key?('retainPrefs')
		move_options = suitesettings['moveOpts'] if suitesettings.key?('moveOpts')
		begin
			TEST_INFO.timeout_factor = Float(suitesettings['timeoutFactor']) if suitesettings.key?('timeoutFactor')
		rescue
			"Invalid timeout factor: #{suitesettings['timeoutFactor']}.".abort(self)
		end
		script_or_suite = suite['tests']
	end
end

target_addr = LOCAL_HOST if target_addr.nil?
target = Target.new(target_addr).load_info

rs = RunSuite.new(script_or_suite, target, delete_prefs, debug_mode, verbose,
  log_elapsed_time, move_options, args, simplified_output, disable_digitrace)

result = SuiteResult::RESULTSTATUS_UNKNOWN

begin
  result = rs.run(custom_log_file, log_dir)
rescue DTTError => err
  err.to_s.log
  err.backtrace.join("\n").log
rescue StandardError => err
  "UNHANDLED ERROR".log
  err.to_s.log
  err.backtrace.join("\n").log
rescue Interrupt => err
  "Runsuite cancelled by user.".log
rescue Exception => err
  "FATAL EXCEPTION".log
  err.to_s.log
  err.backtrace.join("\n").log
end

exit result

