DigiShell for the AAX Toolkit
v. 0.2.1
Dec 6, 2017

Copyright 2014, 2017 Avid Technology, Inc.  All rights reserved.  This
software is part of the Avid Plug-In SDK and is Confidential Information of
Avid Technology, Inc.  Access to and use of this application is restricted to
Avid Third-Party Developers who are validly licensed under the Avid Third
Party Developer and Nondisclosure Agreement (the "Developer Agreement")
subject to the terms and conditions of the Developer Agreement.



=============================================================================
DigiShell (DSH)
=============================================================================


-----------------------------------------------------------------------------
About
-----------------------------------------------------------------------------
DigiShell is a software tool that provides a general framework for running
tests on Avid audio hardware. As a command-line application, DigiShell may be
driven as part of a standard, automated test suite for maximum test coverage.

DigiShell includes a small set of built-in commands. The list of available
commands can be extended using DigiShell modules known as "dishes". Each dish
is designed to provide access to a particular component or to a set of
related features.

This package includes three dishes:
  aaxh.dish - access to the AAXH framework for direct AAX plug-in hosting
  DAE.dish - access to the AAE framework for audio engine operations
  aaxval.dish - access to the AAX Validator framework. See the ReadMe for the
    AAX Validator for more information about this tool.

-----------------------------------------------------------------------------
Usage
-----------------------------------------------------------------------------
Type 'help' at the dsh command prompt to see a full list of the available
commands, or type 'help commandname' to see information for a particular
command

After it is launched, DigiShell waits for a command name and parameters to be
entered via stdin; command results are output via stdout. DigiShell parses
its input as command name, followed by a single space, and then command
parameters.

The native text format for DigiShell is yaml, and all command parameters are
expected to be entered as a yaml-encoded string. Here are two examples of
strings in compact (single-line) yaml format:

  A hash containing lists in compact yaml syntax
  { key1: [val1, val2], key2: [val3, val4] }
  
  A list of two lists
  [[PIO, 0, 1], [DSP, 1, 1]]

DigiShell does not support character escaping. Use quoted strings to pass in
a filepath or other argument that includes special characters such as spaces.

-----------------------------------------------------------------------------
Quick Start
-----------------------------------------------------------------------------
These steps will familiarize you with DigiShell by guiding you through some
basic commands using the aaxh dish

1. Launch bin/dsh (OSX) or dsh.exe (Windows)

2. View the list of built-in DigiShell commands
    dsh> help

3. Load the aaxh dish
    dsh> load_dish aaxh

4. View the updated list of DigiShell commands, including the aaxh dish
commands
    dsh> help

5. Get more information about the "aaxh.loadpi" command
    dsh> help loadpi

6. Load a plug-in from your system
    dsh> loadpi "/absolute/path/to/MyPlugIn.aaxplugin"

7. List the Effects that the plug-in registers
    dsh> listeffects 0

8. View the different options for printing an Effect's description
    dsh> help getdescription

9. Print the first Effect's description in yaml format
    dsh> getdescription {plugin: 0, effect: 0, stringformat: yaml}

-----------------------------------------------------------------------------
Logging
-----------------------------------------------------------------------------
DigiShell supports the DigiTrace logging system. This is the same logging
system that is used by Pro Tools. DigiShell log files are written into a
"Logs" folder next to the dsh executable.

To configure logging for DigiShell, place a DigiTrace config file named
either "dsh.digitrace" or "config.digitrace" next to the dsh executable.

The DigiShell tools package comes pre-loaded with a default configuration
file including most of the tracing facilities that you will need for AAX
plug-in logging. You can add or remove facilities from this file, or change
the logging level for each trace facility to meet your needs.

DigiShell also includes two built-in commands for managing the DigiTrace
configuration at run time:

  enable_trace_facility: Enables a specific trace facility
  dsh> enable_trace_facility ["DTF_AAXPLUGINS", "DTP_NORMAL"]
  
  clear_trace_config: Clear all enabled trace facilities
  dsh> clear_trace_config

For more information about DigiTrace logging configuration and available
trace facilities, see the DigiTrace Guide page in the AAX SDK documentation.

-----------------------------------------------------------------------------
More information
-----------------------------------------------------------------------------
For more information about using DigiShell, see the DSH Guide page in the AAX
SDK documentation



=============================================================================
DishTestTool (DTT)
=============================================================================


-----------------------------------------------------------------------------
About
-----------------------------------------------------------------------------
DishTestTool (DTT) is an automatable command-line test tool that can be used
as a scripting engine for DSH. DTT provides a Ruby scripting layer for
running dsh, loading dishes, and executing commands.

DTT can execute either a single Ruby script or a collection of scripts by
using a specially formatted "suite" file.

-----------------------------------------------------------------------------
More information
-----------------------------------------------------------------------------
For basic DTT usage and compatibility information, see DTT/sources/ReadMe.txt

For additional information, see the DTT Guide page in the AAX SDK
documentation



===============================================================================
Release notes
===============================================================================

0.2.1
    This release fixes an issue that was causing command-line programs such as
    the AAX Validator test programs to frequently crash on macOS 10.13 High
    Sierra. (AAXTOOL-1252)



=============================================================================
Known issues
=============================================================================

-----------------------------------------------------------------------------
DigiShell
-----------------------------------------------------------------------------

AAXTOOL-398
   The built-in DigiShell command "bfsave" creates an invalid audio file
   which cannot be opened by Pro Tools. The file can be successfully loaded
   back into DigiShell.

AAXTOOL-567
   On OS X, the dsh process can hang completely if a crash occurs during
   command execution

AAXTOOL-686
   The offset calculated by the built-in DigiShell command "bcalc_offset" is
   not accurate

-----------------------------------------------------------------------------
DigiShell - DAE dish
-----------------------------------------------------------------------------

PTSW-194115
   DAE.cyclesshared does not warn the user when arguments are ignored
   
   There are no plans to resolve this issue.   

PTSW-191549 and PT-218495
   "DAE.load_wav_file" on Windows results in Sys_FileLocWin32.cpp assertion
   when loading a file from the DigiShell directory (next to dsh.exe)
   
   There are no plans to resolve this issue.

AAXTOOL-652 and PTSW-185528
   AAX Hybrid plug-ins cannot be instantiated in the DAE dish until after
   the "DAE.acquiredeck" command is executed
    
    This bug is due to a design limitation in the DAE dish and it will not be
    resolved.
    
    This series of commands will fail for the DSP configurations of
    AAX Hybrid plug-ins:
      dsh> load_dish DAE
      dsh> run (any Hybrid DSP plug-in)
    
    To work around this limitation, use the acquiredeck command:
      dsh> load_dish DAE
      dsh> init_dae 96000
        If a custom sample rate is required; the default is 44.1kHz
      dsh> acquiredeck
        This command acquires the HDX hardware for the DigiShell process
      dsh> findpi, run, cyclesshared, or any command which requires a plug-in
           index

AAXTOOL-974
  aaxval.rcmp shows differences for some identical results

AAXTOOL-975
   DAE.cyclesshared runs very slowly when adjust_controls is used

AAXTOOL-1138
   The DAE.dae command's 'verif_ctrl_path' argument is not implemented
   
   There are no plans to implement this argument. It will be removed in a
   future build.

AAXTOOL-1178
   The DAE.piproc command results in an error when used with AAX Hybrid
   plug-ins
   
   There are no plans to resolve this issue. The DAE.piproc command is not
   the preferred way to pass audio through plug-ins with the DAE dish.

AAXTOOL-1237
   The DAE dish removes the Avid HDX Core Audio device. After the DAE dish
   has been used, systems with HDX installed will not be able to select
   the Avid HDX device from the Core Audio device list.
   
   This bug does not impact availability of HDX hardware to Pro Tools. It
   only prevents other audio applications in the system from using HDX as a
   Core Audio device.
   
   To make the Core Audio device available again, load the DAE dish and
   execute the DAE.release_hw command.
    
   
-----------------------------------------------------------------------------
DigiShell - aaxh dish
-----------------------------------------------------------------------------

AAXTOOL-178
   "aaxh.instantiate" and "aaxh.instantiateforcontext" allows plug-ins to be
   instantiated with invalid configurations.
   
   To work around this limitation, use "aaxh.geteffectsupportscontext" to
   determine whether a given context is supported before attempting to
   instantiate a plug-in with that context.

-----------------------------------------------------------------------------
DishTestTool
-----------------------------------------------------------------------------

AAXTOOL-855
   The PATH may be cleared before loading a DTT script. This prevents system
   libs from being loaded from within a script. The workaround is to load
   all required system libs in DishTestTool.rb.
