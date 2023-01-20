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
custom_irb="$home/../ruby_dist/bin/irb"
source_dir="$home/sources"

if [ -f "$custom_irb" ]; then
	irb="$custom_irb"
else
	irb="/usr/bin/irb"
fi

if [ ! -d "$source_dir" ]; then
	source_dir="$home/../sources"
fi

eval "$irb -I $(realpath $source_dir) -r dishtesttool -r 'irb/completion' $@"