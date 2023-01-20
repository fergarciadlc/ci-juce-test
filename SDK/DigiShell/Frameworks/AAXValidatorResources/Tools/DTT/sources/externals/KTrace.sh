#!/bin/bash
#version 20.3.17.1 added avbdiagnose
runSysDiagnose=true
loggingOn=false

# options
# -m start or capture
# -b bufferSize for start
# -d runSysDiagnose after Capture
# -l loggingOn

while getopts n:m:b:dl option
do
  case "${option}"
    in
    m) theMode=${OPTARG};; #get mod
    b) bufferSize=${OPTARG};; #bufferSize 2048
    d) runSysDiagnose=true;;
    l) loggingOn=true;;
  esac
done

echo 'KTrace script starting...'
if $loggingOn
then
  echo 'Options: '$theMode $bufferSize 'RunSysDiagnose:'$runSysDiagnose 'Logging:'$loggingOn
fi

if [ "$theMode" == "start" ]
then
  # KTrace Start
  echo 'KTrace starting...'
  ktrace artrace -b $bufferSize -r #> /dev/null
  echo 'KTrace rolling.'

elif [ "$theMode" == "reset" ]
then
  # KTrace Reset
  echo 'KTrace reset starting...'
  ktrace reset #> /dev/null
  echo 'KTrace reset complete.'

elif [ "$theMode" == "capture" ]
then
  # KTrace Capture
  echo 'KTrace Capture starting...'
  # Setup - create folder with timestamp name on desktop example: protools-trace20_01_21_182930
  cDate=( $(date +%y_%m_%d_%H%M%S) )
  mkdir ~/Desktop/protools-trace$cDate
  ktraceLogPath=~/Desktop/protools-trace$cDate
  ktraceFileName='protools-trace.ktrace'
  sleep 0.5

  # Capture
  ktrace dump -E $ktraceLogPath/$ktraceFileName > /dev/null 2>&1
  echo 'KTrace Capture complete.'

  chmod -R 777 $ktraceLogPath

  #cat $ktraceLogPath/ktrace-dump.txt | grep 'could not use existing trace session'
  if test -f $ktraceLogPath/$ktraceFileName
  then
    #echo '$FILE exists'
    ktraceCaptureSuccess=true
  else
    echo 'KTrace capture failed.'
    ktraceCaptureSuccess=false
  fi

  if $ktraceCaptureSuccess
  then
    #avbdiagnose start
    if $loggingOn; then echo 'Avbdiagnose starting...'; fi
    avbdiagnose 2> /dev/null
    latestavbdiagnose=( $(ls -Artd /tmp/*.bz2 | tail -n1 ) )
    cp $latestavbdiagnose $ktraceLogPath/
    if $loggingOn; then echo 'Avbdiagnose complete.'; fi

    # Sysdiagnose
    if $runSysDiagnose
    then
      if $loggingOn; then echo 'Sysdiagnose starting...'; fi
      sysdiagnose -uf $ktraceLogPath > /dev/null
      if $loggingOn; then echo 'Sysdiagnose complete.'; fi
    fi

    # Symbolicate the ktrace file
    if $loggingOn; then echo 'Symbolicating Trace - this may take a few minutes'; fi
    ktrace symbolicate $ktraceLogPath/$ktraceFileName 2> /dev/null
    if $loggingOn; then echo 'Symbolicating complete.'; fi

    # Save System Profile
    if $loggingOn; then echo 'Saving System Profile...'; fi
    theHostName=( $(hostname) )
    system_profiler -detailLevel full -xml 1> $ktraceLogPath/SystemProfile_$theHostName.spx 2> /dev/null
    if $loggingOn; then echo 'Saving System Profile complete.'; fi

    # Copy latest dlog
    if $loggingOn; then echo 'Copying latest Dlog to ' $ktraceLogPath; fi
    latestDLogFile=( $(ls -Artd ~/Library/Logs/Avid/Pro_Tools*txt | tail -n1 ) )
    cp $latestDLogFile $ktraceLogPath/

    # Zip it
    if $loggingOn; then echo 'Zipping '$ktraceLogPath; fi
    zip -rj $ktraceLogPath.zip $ktraceLogPath > /dev/null
    sleep 0.5
    if $loggingOn; then echo 'Zip complete'; fi

    # Cleanup
    rm -fr $ktraceLogPath

    if $ktraceCaptureSuccess
    then
      echo 'KTrace capture completed successfully.'
    fi
  fi
fi
