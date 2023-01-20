DigiShell for the AAX Toolkit
12/02/2014

Copyright 2014 Avid Technology, Inc.  All rights reserved.  This software is
part of the Avid Plug-In SDK and is Confidential Information of Avid
Technology, Inc.  Access to and use of this application is restricted to Avid
Third-Party Developers who are validly licensed under the Avid Third Party
Developer and Nondisclosure Agreement (the "Developer Agreement") subject to
the terms and conditions of the Developer Agreement.



============================================================================
DigiShell (DSH)
============================================================================


----------------------------------------------------------------------------
About
----------------------------------------------------------------------------
DigiShell is a software tool that provides a general framework for running
tests on Avid audio hardware. As a command-line application, DigiShell may be
driven as part of a standard, automated test suite for maximum test coverage.

DigiShell includes a small set of built-in commands. The list of available
commands can be extended using DigiShell modules known as "dishes". Each dish
is designed to provide access to a particular component or to a set of
related features.

This package includes two dishes:
  aaxh.dish - access to the AAXH framework for direct AAX plug-in hosting
  DAE.dish - access to the AAE framework for audio engine operations

----------------------------------------------------------------------------
Usage
----------------------------------------------------------------------------
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

----------------------------------------------------------------------------
Quick Start
----------------------------------------------------------------------------
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

----------------------------------------------------------------------------
More information
----------------------------------------------------------------------------
For more information about using DigiShell, see the DSH Guide page in the AAX
SDK documentation



============================================================================
DishTestTool (DTT)
============================================================================


----------------------------------------------------------------------------
About
----------------------------------------------------------------------------
DishTestTool (DTT) is an automatable command-line test tool that can be used
as a scripting engine for DSH. DTT provides a Ruby scripting layer for
running dsh, loading dishes, and executing commands.

DTT can execute either a single Ruby script or a collection of scripts by
using a specially formatted "suite" file.

----------------------------------------------------------------------------
More information
----------------------------------------------------------------------------
For basic DTT usage and compatibility information, see DTT/sources/ReadMe.txt

For additional information, see the DTT Guide page in the AAX SDK
documentation



============================================================================
Known issues
============================================================================

----------------------------------------------------------------------------
DigiShell
----------------------------------------------------------------------------

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

----------------------------------------------------------------------------
DigiShell - DAE dish
----------------------------------------------------------------------------

AAXTOOL-652
   AAX Hybrid plug-ins cannot be instantiated in the DAE dish until after
   the "DAE.acquiredeck" command is executed
   
AAXTOOL-734
   "DAE.run" fails to load a plug-in if the plug-in DLL has previously been
   loaded using the "aaxh.loadpi" command
