==================================
README
DishTestTool
==================================


Introduction
------------

This directory contains DishTestTool (DTT), an automatable command-line test
tool that can be used as a scripting engine for DSH.


Confidentiality Agreement
-------------------------

Copyright 2014, 2017 Avid Technology, Inc.  All rights reserved.  This software
is part of the Avid Plug-In SDK and is Confidential Information of Avid
Technology, Inc.  Access to and use of this application is restricted to Avid
Third-Party Developers who are validly licensed under the Avid Third Party
Developer and Nondisclosure Agreement (the "Developer Agreement") subject to
the terms and conditions of the Developer Agreement.


Installation and System Requirements
------------------------------------

DigiShell
	
	DigiShell must be installed at a specific path relative to the DTT folder:
	
		Mac: DTT/../CommandLineTools/dsh
		Win: DTT\..\dsh.exe

Ruby
	
	DTT requires Ruby. Ruby must be either installed on the system or provided
	as a stand-alone distribution.
	
	This build of DTT is compatible with Ruby versions from 1.9.3 up to but not
	including 2.2.
	
	Ruby comes pre-installed on OS X, so no specific Ruby installation is
	required on Mac systems.
	
	Ruby for Windows must be downloaded separately. If you wish to use the
	system's installed copy of Ruby for DTT then you must add the path to
	ruby.exe to your system's PATH variable.
	
		http://rubyinstaller.org/downloads/
	
	You may install a stand-alone build of Ruby if you do not wish to change
	the Ruby configuration of your system.
	
	DTT first searches for Ruby in a ruby_dist folder next to the DTT directory.
	If Ruby is not found in this location then DTT searches for Ruby within the
	system's PATH.
	
	Some distributions of DTT (e.g. with the AAX Validator) contain a
	stand-alone build of Ruby in a ruby_dist folder which may be copied next to
	the DTT directory.


HDX

	Before running tests on HDX hardware you will need to be sure that the HDX
	hardware drivers are installed, that the OS can detect the card, and that a
	compatible I/O peripheral is connected and powered on.


Usage
-----

Mac: DTT/run_test.command [--script] NameOfScriptOrSuite [options]
Win: DTT\run_test.bat [--script] NameOfScriptOrSuite [options]

NameOfScriptOrSuite
  The name of a Script class from DTT/sources/scripts or a suite file from
  DTT/sources/suites, without the file's .gss suffix.

Script arguments
  Script arguments may be explicitly defined using the -a option. Other
  options are documented in the DTT help text message (see -h below.)

Usage Notes
  * When specifying a file path to a script using the -a option on Windows, do
    not enclose the path itself in quotes.
    
    Example:
    run_test.bat --script MyScript -a 'path_arg=C:\not a quoted path'
    


Main Options
------------

-h
	show help message.

-l
	return list of tests with corresponding number. So, you can use test name as
	well as corresponding number.

-v
	turn on verbose mode. All debug information will be sent to console.

-o
	Retain options. Don't delete DigiOptionsFile.txt before each test


Logging
-------

During each test, DTT logs all information. It creates a catalog in the "log"
directory with the test's name + date stamp.

Logging Types:

	_l
		standard log.
	
	_v
		verbose log.
	
	_d
		Human-readable log with all trace from DTT to dsh and vice versa.
