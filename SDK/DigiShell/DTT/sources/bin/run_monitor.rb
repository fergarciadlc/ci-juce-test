#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

require 'pathname'
require 'optparse'
require 'rbconfig'

$:.push(Pathname.new($0).parent.parent.to_s)

if File.fnmatch("* *", $:.last) && /apple/.match(Config::CONFIG['host'])
  puts "\nWARNING: Dish Test Tool may not operate properly if there is a space in its path.\n"
end

require 'DishTestTool'
include DishTestTool

build_folder = nil
verbose = false

opts = OptionParser.new
opts.on('-h', '--help') { puts opts; exit }
opts.on('-p', '--path BUILD') { |str| build_folder = Pathname.new(str) }
opts.on('-v', '--verbose') { verbose = true }
opts.parse(ARGV)

begin
  DTTMonitor.new(build_folder, verbose).run
rescue DTTError => err
  puts err.to_s
  exit 1
end


