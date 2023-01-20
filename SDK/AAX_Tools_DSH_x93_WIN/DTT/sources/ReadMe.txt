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

This software is subject to the confidentiality agreement that you have signed 
with Avid.  Do not distribute this software to other parties or discuss any Avid 
technology with other parties until such technology becomes public.


Installation and System Requirements
------------------------------------

Ruby
	
	DTT requires Ruby versions up to 1.9.3.
	
	Ruby comes pre-installed on OS X, so no specific Ruby installation is
	required on Mac systems.
	
	To run DTT on Windows, you will need to install Ruby and add the path to
	ruby.exe to your system's PATH variable.
	
		http://rubyinstaller.org/downloads/


HDX

	Before running tests on HDX hardware you will need to be sure that the HDX
	hardware drivers are installed, that the OS can detect the card, and that a
	compatible I/O peripheral is connected and powered on.


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


