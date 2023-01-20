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
#    -n, --no_pref_delete
#    -l, --list
#    -t, --target ADDR
#    -v, --verbose
#    -x, --timeout_factor VAL
#    -z, --zeta_device_logs
#    -k, --gather_ktrace
#    -r, --export_logs
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
#  -z, --zeta_device_logs:
#  Gather Zeta logs after all test failures
#
#  -k, --gather_ktrace:
#  Gather ktrace logs after all test failures
#
#  -r, --export_logs:
#  Export compressed DTT test's "logs" folder to HOME directory. Useful to investigate failing tests  
#
#  == Author
#  Tim Walters, Digidesign
#
#  == Copyright
#  Copyright 2017 by Avid Technology, Inc.

require_relative '../DishTestTool'

def warn(str)
  str.abort
end  

def get_idx_scipt_list
  gss_files = SUITE_DIR.children(false).select {|suite| suite.extname == '.gss'}
  suites = gss_files.collect {|suite| suite.to_s.sub(suite.extname, '')}

  idx_script = {}
  scripts  = suites + TEST_SCRIPTS.to_a.collect{|script| script.to_s}.sort
  scripts.each_with_index {|s, idx| idx_script[idx] = s}

  return idx_script
end

def get_script_by_idx(idx)
  idx = idx.to_i if idx.instance_of?(String)
  get_idx_scipt_list[idx]
end

def dump_idx_script_list
  get_idx_scipt_list.each_pair {|i, s| puts "#{i} => #{s}"}
end

args = {}
custom_log_file = nil
debug_mode = false
delete_prefs = true
disable_digitrace = false
log_dir = nil
log_elapsed_time = true
move_options = true
pb_port = nil
script = nil
simplified_output = false
target_addr = nil
verbose = false
zeta_device_logs = false
ktrace_logs = false
export_logs = false

opts = OptionParser.new
opts.on('--script SCRIPT') {|s| script = s}
opts.on('-d', '--debug') { debug_mode = true }
opts.on('-e', '--no_log_elapsed_time') { log_elapsed_time = false }
opts.on('-f', '--file FILE') { |str| custom_log_file = Pathname.new(str) }
opts.on('-g', '--logdir LOGDIR') { |str| log_dir = Pathname.new(str) }
opts.on('-h', '--help') { puts opts; exit }
opts.on('-l', '--list') { dump_idx_script_list; exit }
opts.on('-n', '--no_pref_delete') { delete_prefs = false }
opts.on('-o', '--no_move_options') { move_options = false }
opts.on('-p', '--port PORT') {|p| pb_port = p}
opts.on('-s', '--simplified_output') { simplified_output = true }
opts.on('-t', '--target ADDR') { |addr| target_addr = addr }
opts.on('-v', '--verbose') { verbose = true }
opts.on('-w', '--disable_digitrace') {disable_digitrace = true}
opts.on('-x', '--timeout_factor VAL') {|str| TEST_INFO.timeout_factor = Float(str)}
opts.on('-z', '--zeta_device_logs') {zeta_device_logs = true}
opts.on('-K', '--gather_ktrace_m') do
  $stdout.print "Enter User password: "
  TEST_INFO.user_pwd = $stdin.noecho(&:gets)
  TEST_INFO.user_pwd.strip!
  puts "Your password was #{TEST_INFO.user_pwd.length} characters long."
  ktrace_logs = true
end
opts.on('-k', '--gather_ktrace PWD') do |str|
  TEST_INFO.user_pwd = str
  ktrace_logs = true
end
opts.on('-r', '--export_logs') {export_logs = true}
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

remainder = opts.parse(ARGV)[0]

if remainder.nil? and script.nil?
  puts opts
  exit
elsif remainder.nil? and !script.nil?
  remainder = script
end

# we have to convert idx to script-name for further exectution
remainder = get_script_by_idx(remainder) if remainder =~ /^\d+$/

scripts = []
suites = []

if Pathname.new(remainder).directory?
  suites += Pathname.new(remainder).children.select{|s| s.extname == '.gss'}
elsif (SUITE_DIR + (remainder + ".gss")).exist?
  suites << (SUITE_DIR + (remainder + ".gss"))
elsif Pathname.new(remainder).exist? and (Pathname.new(remainder).extname == '.gss')
  suites << remainder
end

if suites.empty?
  scripts << remainder
else
  suites.each do |suite_path|
    suite = YAML::load(IO.read(suite_path))
    suitesettings = suite['suitesettings']

    delete_prefs = !suitesettings['retainPrefs'] if suitesettings.key?('retainPrefs')
    move_options = suitesettings['moveOpts'] if suitesettings.key?('moveOpts')
    verbose = suitesettings['verbose'] if suitesettings.key?('verbose')

    TEST_INFO.timeout_factor = Float(suitesettings['timeoutFactor']) if suitesettings.key?('timeoutFactor')

    scripts += suite['tests']
  end
end

target_addr = LOCAL_HOST if target_addr.nil?
target = Target.new(target_addr).load_info

rs = RunSuite.new(scripts, target, delete_prefs, debug_mode, verbose,
  log_elapsed_time, move_options, args, simplified_output, disable_digitrace, pb_port, zeta_device_logs, ktrace_logs, export_logs)

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