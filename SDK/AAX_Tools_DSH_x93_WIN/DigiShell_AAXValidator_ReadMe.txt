AAX Plug-In Validator
v. 0.1
Jan 22, 2015

Copyright 2014 Avid Technology, Inc.  All rights reserved.  This software is
part of the Avid Plug-In SDK and is Confidential Information of Avid
Technology, Inc.  Access to and use of this application is restricted to Avid
Third-Party Developers who are validly licensed under the Avid Third Party
Developer and Nondisclosure Agreement (the "Developer Agreement") subject to
the terms and conditions of the Developer Agreement.



===============================================================================
AAX Plug-In Validator and aaxval dish
===============================================================================


-------------------------------------------------------------------------------
About
-------------------------------------------------------------------------------
The AAX Plug-In Validator consists of a test runner framework and a set of test
modules. The AAX Plug-In Validator is designed to help AAX plug-in developers
test, debug, and verify plug-ins.

The aaxval dish is a component for the DigiShell application. This dish
provides a command-line interface to the AAX Plug-In Validator framework.

Interface details for the test runner framework are available in
AAXValidator.framework/Headers in the OS X version of this package.


-------------------------------------------------------------------------------
Requirements
-------------------------------------------------------------------------------
Operating System

	OS X 10.8 or higher
	Windows 7 or higher


Plug-ins

	AAX 64-bit


HDX

	Before running tests on HDX hardware you will need to be sure that the HDX
	hardware drivers are installed, recognized by OS, and that a compatible I/O
	peripheral is connected and powered on.


Plug-in authorizations

	Before running tests for copy-protected plug-ins make sure that the
	test system contains proper authorizations for these plug-ins.


-------------------------------------------------------------------------------
Quick Start
-------------------------------------------------------------------------------
Note: For more information about DigiShell (DSH), see the DigiShell ReadMe file

1.  Launch bin/dsh (OSX) or dsh.exe (Windows)

2.  Load the aaxval dish
    	dsh> load_dish aaxval

3.  View the updated list of DigiShell commands, which now includes commands
    from the aaxval dish
    	dsh> help

4.  View the list of available tests. Each test has a name, a brief descripton,
    and an id
    	dsh> listtests

5.  Get more information about the aaxval.runtest and aaxval.runtests commands
    	dsh> help runtest
    	dsh> help runtests

6.  Run a single test on a single plug-in
    	dsh> runtest [test.data_model, "/absolute/path/to/MyPlugIn.aaxplugin"]
    		or
    	dsh> runtest {test: test.data_model, path: "/absolute/path/to/MyPlugIn.aaxplugin"} 

7.  Run a single test on all AAX plug-ins inside a target directory
		dsh> runtest [test.data_model, "/absolute/path/to/target/directory"]

8.  Run all available tests on a single plug-in or on all plug-ins in a target
    directory
		dsh> runtests "/absolute/path/to/MyPlugIn.aaxplugin"
		dsh> runtests "/absolute/path/to/target/directory"

9.  Display the complete validator configuration, including test modules,
    collections, and categories
		dsh> getconfig

10. List all tests from the "Plug-in configuration" collection, then list all
    tests from the "Plug-in data model tests" module that are categorized as
    "Reliability tests"
		dsh> listtests {coll: col_tests_config}
		dsh> listtests {cat: cat_reliability, mod: mod_tests_component_datamodel}

11. Run all plug-in configuration tests on a single plug-in or on all plug-ins
    in a target directory
		dsh> runtests {coll: col_tests_config, path: "/absolute/path/to/MyPlugIn.aaxplugin"}
		dsh> runtests {coll: col_tests_config, path: "/absolute/path/to/target/directory"}


-------------------------------------------------------------------------------
Usage Notes
-------------------------------------------------------------------------------

* As demonstrated above in the Quick Start guide, the Validator organizes tests
  into modules, collections, and categories.
  
  * Module: A group of tests that are designed to be run together in a single
    test event. Each test belongs to zero or more modules.
  * Collection: A high-level group of tests that are topically related. Each
    test module belongs to zero or more collections.
  * Category: An area of test coverage that is relevant to plug-in users. Each
    test belongs to zero or more categories.
  
  The aaxval dish provides options to list and execute tests from any
  combination of module, collection, and category. Use the 'getconfig' command
  to print the current Validator configuration, including its test organization
  
* Test results can be saved to disk, then loaded at a later time using the
  'saveresult' and 'loadresult' commands.
  
* The native output format for the aaxval dish is YAML. Other formats (XML,
  JSON) are available for many commands via the "stringformat" parameter. For
  example, to run a test and print the test results in XML:
	dsh> runtest {test: test.data_model, path: "/absolute/path/to/MyPlugIn.aaxplugin", stringformat: xml}

* Tests running on Windows require more time to complete in comparison to OS X

===============================================================================
Known issues
===============================================================================

1.  RELENG-1167
    dsh.exe is not signed on Windows, and Windows will present an "Unknown
    Publisher" dialog when DSH is first run

2.  AAXTOOL-567
    DSH may hang after unsuccessful command execution. This is an intermittent
    behavior
 
3.  AAXTOOL-653
    The aaxval dish fails to merge results from tests run in parallel and
    raises an assertion failure:
      'Assertion failed: (true == success), function MergeImpl...'
    To avoid this, always run tests in series (the default setting)

4.  AAXTOOL-689
    Occasionally a test run will terminate with an "Invalid destination port"
    error. This issue occurs most often when using the 'runtests' command to
    execute multiple tests.

5.  AAXTOOL-692
    'test.parameter_traversal.linear' may generate a debug assert upon
    completion. The assert is benign in this context.

6.  AAXTOOL-771
    'test.cycle_counts' takes a long time to execute. The execution time
    depends on the plug-in's characteristics such as supported stem formats,
    sample rates, and the number of factory presets. The average execution time
    is about 1 hour. Because of its long execution time, this test may time out
    on Windows with status E_ABORTED.
