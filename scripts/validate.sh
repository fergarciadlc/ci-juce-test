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

echo "OS: ${OSTYPE}"
echo "Validating ${PLUGIN_PATH}"

if $pluginval --strictness-level 5 --validate-in-process --validate $PLUGIN_PATH --output-dir pluginval-logs --timeout-ms 500000;
then
    echo "------------------------------------------------------------------------------------------------------------------------------"
    echo "Pluginval for ${PLUGIN_PATH} succeeded"
    echo "------------------------------------------------------------------------------------------------------------------------------"
else
    echo "------------------------------------------------------------------------------------------------------------------------------"
    echo "Pluginval for ${PLUGIN_PATH} failed"
    echo "------------------------------------------------------------------------------------------------------------------------------"
    cat pluginval-logs/*
    exit 1
fi