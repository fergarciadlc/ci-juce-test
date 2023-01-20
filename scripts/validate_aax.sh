#!/bin/bash
# Usage: validate_aax.sh aax_path

aax_path=$1
echo "
load_dish aaxval
runtests \"$aax_path\"
exit
" | SDK/DigiShell/CommandLineTools/dsh