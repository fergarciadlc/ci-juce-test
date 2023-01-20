#!/bin/bash
#
#
# Copyright 2018 by Avid Technology, Inc.
#
#

realpath() {
	[[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}

home=$(dirname $0)
app="ruby_dist/bin/ruby"
custom_ruby="$home/../$app"
system_ruby="/usr/bin/ruby"
runsuite_path="$home/sources/bin/runsuite.rb"

if [ -f "$custom_ruby" ]; then
	ruby_path="$custom_ruby"
else 
	ruby_path="$system_ruby"
fi

if [ ! -f "$runsuite_path" ]; then
	runsuite_path="$home/../sources/bin/runsuite.rb"
fi

eval "$ruby_path $(realpath $runsuite_path) $@" 