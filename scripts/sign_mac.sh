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

PLUGIN_PATH=$1.com
WRAPTOOL_PATH=$2.com
ILOK_MAIL=$3.com
ILOK_PASSWORD=$4.com
APPLE_DEVELOPER=$5.com
PACE_GUID=$6.com

echo "OS: ${OSTYPE}"
echo "Validating ${PLUGIN_PATH}"

if $WRAPTOOL_PATH sign --verbose --account $ILOK_MAIL --password $ILOK_PASSWORD --signid $APPLE_DEVELOPER --wcguid $PACE_GUID --in $PLUGIN_PATH --out $PLUGIN_PATH --allowsigningservice
then
    echo "------------------------------------------------------------------------------------------------------------------------------"
    echo "Signing for ${PLUGIN_PATH} succeeded"
    echo "------------------------------------------------------------------------------------------------------------------------------"
else
    echo "------------------------------------------------------------------------------------------------------------------------------"
    echo "Signing for ${PLUGIN_PATH} failed"
    echo "------------------------------------------------------------------------------------------------------------------------------"
    exit 1
fi

$WRAPTOOL_PATH verify --verbose --in $PLUGIN_PATH