#!/bin/bash
#
#   Created by Sergii Anisienko in 2020
#   Copyright (c) 2020 Avid Technology. All rights reserved.
#
#   two arguments should be passed to this script:
#   $1 - testbed's zeta device ID ex. zeta1234
#   $2 - work directory
#   $3 - build type
#

##################
#####clean up#####
##################
osascript -e 'quit app "Terminal"'

###########################
#####set global params#####
###########################
ZETA_ID=$1
WORK_ROOT=$2
BUILD_TYPE=$3
TIMESTAMP=$( date '+%F_%H:%M:%S' )

######################
#####show warning#####
######################
open -a Terminal "$WORK_ROOT/$BUILD_TYPE/DTT/sources/externals/warning.command"

#######################
#####open monitors#####
#######################
open -a Terminal "$WORK_ROOT/$BUILD_TYPE/DTT/run_monitor.command"
# open -a Terminal "/usr/local/Cellar/minicom/2.7.1/bin/minicom"
open -a Terminal "/Volumes/Data/workRoot/tests/Artifact/Release/CommandLineTools/Cloud/scripts_v2.1/DeletePreferences.command" 

###########################
#####update FW version#####
###########################

$WORK_ROOT/$BUILD_TYPE/DTT/run_test.command -t 127.0.0.1 Zeta_FWUpdateTest

#############################
#####set verbose logging#####
#############################
ssh root@$ZETA_ID.local "umount /etc"
ssh root@$ZETA_ID.local "mount -o remount,rw /"
scp $WORK_ROOT/$BUILD_TYPE/DTT/sources/externals/zeta-app.digitrace root@$ZETA_ID.local:/etc/

################################
#####reset persistent state#####
################################
ssh root@$ZETA_ID.local /etc/init.d/S75runapp stop
ssh root@$ZETA_ID.local rm -r /opt/zeta-app-state/*

#############################
#####reboot before start#####
#############################

ssh root@$ZETA_ID.local reboot
sleep 60

########################
#####run test suite#####
########################
