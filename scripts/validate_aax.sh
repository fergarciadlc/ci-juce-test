#!/bin/bash

echo '
  ______              _____                _         _______        _     
 |  ____|            / ____|              | |       |__   __|      | |    
 | |__   __ _ _ __  | |     __ _ _ __   __| |_   _     | | ___  ___| |__  
 |  __| / _  |  __| | |    / _  |  _ \ / _` | | | |    | |/ _ \/ __|  _ \ 
 | |___| (_| | |    | |___| (_| | | | | (_| | |_| |    | |  __/ (__| | | |
 |______\__,_|_|     \_____\__,_|_| |_|\__,_|\__, |    |_|\___|\___|_| |_|
                                              __/ |                       
                                             |___/   
'

PLUGIN_PATH=$1

echo "Validating ${PLUGIN_PATH}"

echo "
load_dish aaxval
runtest [info.productids, ${PLUGIN_PATH}]
runtest [info.support.audiosuite, ${PLUGIN_PATH}]
runtest [info.support.general, ${PLUGIN_PATH}]
runtest [info.support.s6_feature, ${PLUGIN_PATH}]
runtest [test.load_unload, ${PLUGIN_PATH}]
runtest [test.page_table.automation_list, ${PLUGIN_PATH}]
runtest [test.parameter_traversal.linear, ${PLUGIN_PATH}]
runtest [test.parameter_traversal.random, ${PLUGIN_PATH}]
runtest [test.parameter_traversal.random.fast, ${PLUGIN_PATH}]
runtest [test.parameters, ${PLUGIN_PATH}]
exit
" | DigiShell/CommandLineTools/dsh

