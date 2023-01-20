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

PLUGIN_NAME={1}
FORMAT={2}

# install functions
install_pluginval_mac()
{
    curl -L "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_macOS.zip" -o pluginval.zip
    unzip pluginval > /dev/null
    echo "pluginval.app/Contents/MacOS/pluginval"
}

install_pluginval_win()
{
    powershell -Command "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_Windows.zip -OutFile pluginval.zip"
    powershell -Command "Expand-Archive pluginval.zip -DestinationPath ."
    echo "./pluginval.exe"
}

# install
if [[ "$OSTYPE" == "darwin"* ]]; then
    pluginval=$(install_pluginval_mac)
    declare -a plugins=([1]="/Users/runner/Library/Audio/Plug-Ins/VST3/ci-cmake-juce.vst3"
                        [2]="/Users/runner/Library/Audio/Plug-Ins/VST/ci-cmake-juce.vst"
                        [3]="/Users/runner/Library/Audio/Plug-Ins/Components/ci-cmake-juce.component")
else
    pluginval=$(install_pluginval_win)
    declare -a plugins=([1]="D:\a\ci-juce-test\ci-juce-test\build\ci-cmake-juce_artefacts\Release\VST3\ci-cmake-juce.vst3"
                        [2]="D:\a\ci-juce-test\ci-juce-test\build\ci-cmake-juce_artefacts\Release\VST\ci-cmake-juce.dll")
fi

echo "Pluginval installed at ${pluginval}"
${pluginval} --version

# run
for plugin in "${plugins[@]}"; do
    echo ""
    echo "------------------------------------------------------------------------------------------------------------------------------"
    echo "------------------------------------------------------------------------------------------------------------------------------"
    echo "Validating ${plugin}"
    echo "------------------------------------------------------------------------------------------------------------------------------"
    echo "------------------------------------------------------------------------------------------------------------------------------"
    echo ""

    if format in plugin name
      validalo
    else
      siguiente iteracion

    if $pluginval --strictness-level 5 --validate-in-process --validate $plugin --output-dir pluginval-logs --timeout-ms 500000;
    then
      echo "------------------------------------------------------------------------------------------------------------------------------"
      echo "Pluginval for ${plugin} succeeded"
      echo "------------------------------------------------------------------------------------------------------------------------------"
    else
      echo "------------------------------------------------------------------------------------------------------------------------------"
      echo "Pluginval for ${plugin} failed"
      echo "------------------------------------------------------------------------------------------------------------------------------"
      cat pluginval-logs/*
      exit 1
    fi
done

# clean up
rm -Rf pluginval*

bash validate_std.sh [pugin-name] [format]
