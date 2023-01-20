#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

require 'pathname'
require 'rbconfig'
require 'fileutils'
require 'yaml'
require 'net/telnet'
require 'timeout'
require 'socket'
require 'net/smtp'
require 'set'
require 'cgi'
require 'tempfile'
require 'rexml/document'

config = (RUBY_VERSION < "1.9.3") ? "Config" : "RbConfig"
require 'win32ole' unless /apple/.match(eval("#{config}::CONFIG['host']"))

module DishTestTool

  # :stopdoc:
  # If we got here, our folder must be in the lib path.
  SOURCE_DIR = Pathname.new($:.find {|dir| /DTT[\/\\]sources/.match(dir)}).realpath
  BIN_DIR = SOURCE_DIR + 'bin'
  SCRIPT_DIR = SOURCE_DIR + 'scripts'
  SUITE_DIR = SOURCE_DIR + 'suites'
  EXTERNAL_DIR = SOURCE_DIR + 'externals'
  CFG_DIR = SOURCE_DIR + 'machine_cfg'
  CLASS_DIR = SOURCE_DIR + 'classes'
  GS_AUDIO_EXTENSION = ".wav"
    
  MONITOR_PORT = 24000
  PROTOBUFF_PORT = 63036
  LOCAL_HOST = 'localhost'

  #changing working directory to be consistent for relative path
  Dir.chdir(SOURCE_DIR.parent.to_s)

  core_libs = []
  core_libs.push($:.pop) while !$:.empty?
    
  [SCRIPT_DIR, CLASS_DIR, EXTERNAL_DIR].each{|d| $:.push(d.to_s)}

  # Include guard only works if the require is called with the same string.
  # So we add subdirectories to the lib path, and always require with the
  # file name only.
  loadFiles = proc {
      |dir, optional|
    if dir.exist?
      $:.push(dir.to_s) unless $:.include?(dir.to_s)
      dir.children.each do
      |file|
        if file.directory? then
          loadFiles.call(file, optional)
        else
          if file.extname == '.rb' then
            if optional
              begin
                require file.basename
              rescue Exception => err
                puts "WARNING: file #{file} could not be loaded: #{err}"
              end
            else
              require file.basename
            end
          end
        end
      end
    end
  }
  
  loadFiles.call(CLASS_DIR, false)

  # We don't want a bad script and externals to stop everything so we make
  # the scripts and externals optional. 
  loadFiles.call(SCRIPT_DIR, true)
  
  #restoring paths for core ruby libs
  $:.push(core_libs.pop) while !core_libs.empty?

  loadFiles.call(EXTERNAL_DIR, true)

  # After all classes are loaded, get runtime info
  LOG_PARENT_DIR = SOURCE_DIR.parent
  LOG_DIR = LOG_PARENT_DIR + 'logs'
  
  TIME_INFO = TimeInfo.new
  TEST_INFO = TestInfo.new
  LOG_MANAGER = LogManager.new
  RUNTIME_INFO = RuntimeInfo.new
  TEST_SCRIPTS = RUNTIME_INFO.known_scripts

  def wait_for(timeout = 3)
    begin
      Timeout::timeout(timeout.or_if_nil(3) * TEST_INFO.timeout_factor.or_if_nil(1)) do
        while !yield
            sleep(0.1)
        end
        return true
      end
    rescue TimeoutError
      return false
    end
  end

=begin rdoc
Standardizes Goldsmith error handling. Raises an exception which will be caught by the script runner,
terminating the script with message +msg+. The caller of this method will be at the top of the stack.
Normally called via String#abort, which passes in the correct caller.
=end
  def raise_dtt_err(msg, in_caller)
    raise(DTTError, msg, in_caller.or_if_nil(caller))
  end
  
end

