AAX Plug-In Validator
v. 0.2.1
Dec 6, 2017

Copyright 2014, 2015, 2017 Avid Technology, Inc.  All rights reserved.  This
software is part of the Avid Plug-In SDK and is Confidential Information of
Avid Technology, Inc.  Access to and use of this application is restricted to
Avid Third-Party Developers who are validly licensed under the Avid Third Party
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

Like all dishes, the aaxval dish may be scripted with Ruby using the DTT
framework. An example DTT script that runs all Validator tests on a target
directory is provided at DTT/sources/scripts/ValidatorRunAllTests.rb

Interface details for the test runner framework are available in
AAXValidator.framework/Headers in the OS X version of this package. This
framework supports a simple C interface on both Mac and Windows, and it is
possible to create custom tools that directly incorporate this framework.


-------------------------------------------------------------------------------
Requirements
-------------------------------------------------------------------------------
Operating System

    OS X 10.8 or higher, tested on macOS 10.13.1
    Windows 7 or higher, tested on Windows 10


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
        dsh> runtest {test: test.data_model, path: "/absolute/path/to/MyPlugIn.aaxplugin", stringformat: yaml} 

7.  Run a single test on all AAX plug-ins inside a target directory
        dsh> runtest [test.data_model, "/absolute/path/to/target/directory"]

8.  Run all available tests on a single plug-in or on all plug-ins in a target
    directory
        dsh> runtests "/absolute/path/to/MyPlugIn.aaxplugin"
        dsh> runtests "/absolute/path/to/target/directory"

9.  Display the complete validator configuration, including test modules,
    collections, and categories. See Usage Notes below for more information
    about test organization.
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
Tests
-------------------------------------------------------------------------------

A listing of all available tests can be generated using the 'listtests'
command.

There are two broad types of tests: validation tests and information-gathering
tests.

* Validation tests check that the plug-in meets some aspect of the AAX
  specification and provide a success or failure result and logging to report
  the result of the check. Validation tests' IDs are prefixed with "test".

* Information-gathering tests exist to collect and print information about a
  plug-in binary. These tests will typically always succeed so the most
  important artifact of these tests is the text data printed to the result.
  Information-gathering tests' IDs are prefixed with "info".


Information-gathering tests

    info.productids

        Retrieves and displays literal manufacturer and product IDs

        This test is useful for getting product ID information when on-boarding
        plug-ins to Avid Marketplace.

    info.support.audiosuite

        Displays AudioSuite feature support information for all Effects in the
        plug-in

    info.support.general

        Retrieves and displays basic information about the all Effects in a
        plug-in bundle

    info.support.s6_feature

        Tests support of Avid S6 features and provides a score


Validation tests

    test.cycle_counts

        Test cycle counts reported by an AAX DSP plug-in

        This test provides a more comprehensive test than the base
        DAE.cyclesshared command

    test.data_model

        Instantiates and de-instantiates each Effect in a plug-in using all
        supported host contexts


    test.describe_validation

        Validation of the plug-in's configuration (Describe function)

    test.load_unload

        Loads/unloads plug-in binary 1000 times

    test.page_table.automation_list

        Tests the parameter automation list in a plug-in's page tables

    test.page_table.load

        Tests parsing and loading a plug-in's page table XML

    test.parameter_traversal.linear

        Linear test of the plug-in's parameter space with a maximum 5-minute
        execution per Effect

    test.parameter_traversal.random

        Random test of the plug-in's parameter space with a 30-second execution
        per Effect

    test.parameter_traversal.random.fast

        Random test of the plug-in's parameter space with a 3-second execution
        per Effect

    test.parameters

        Validates the behavior of parameters registered by each variant of the
        plug-in


-------------------------------------------------------------------------------
Usage Notes
-------------------------------------------------------------------------------

* As demonstrated in the Quick Start guide above, the Validator organizes tests
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

* DigiShell does not support character escaping. Use quoted strings to pass in
  a filepath or other argument that includes special characters such as spaces.

* Test results can be saved to disk, then loaded at a later time using the
  'saveresult' and 'loadresult' commands.
  
* Test results can be compared against a template using the 'rcmp' command. This
  can be especially helpful for automated testing. The results should be stored
  in a text file and loaded using 'loadresult'. Any loaded result may be used
  by 'rcmp' as a template. The template may be a partial or complete match.
  
  Example 1: This result template will match all successful test results:
  
    result_status: E_COMPLETED_PASS
  
  Example 2: This result template will match info.support.audiosuite test
  results for plug-ins that pass and do not implement AAX_IHostProcessor:
  
    result {
      tree {
        data: "host_processor"
        tree {
          data: "no"
        }
      }
    }
    result_status: E_COMPLETED_PASS
  
* The native output format for the aaxval dish is YAML. Other formats (XML,
  JSON) are available for many commands via the "stringformat" parameter. For
  example, to run a test and print the test results in XML:
    dsh> runtest {test: test.data_model, path: "/path/MyPlugIn.aaxplugin", stringformat: xml}

* Tests running on Windows require more time to complete in comparison to OS X


===============================================================================
Release notes
===============================================================================

0.2.1
    This release fixes an issue that was causing command-line programs such as
    the AAX Validator test programs to frequently crash on macOS 10.13 High
    Sierra. (AAXTOOL-1252)


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

7.  AAXTOOL-887
    info.support.general results cannot be printed to XML

8.  AAXTOOL-893
    The Validator does not ignore "special" plug-ins such as
    DigiReWire.aaxplugin and Mixer.aaxplugin. Tests which are written for AAX
    audio effects may not apply to or succeed when run on these plug-ins.

9.  AAXTOOL-934
    test.page_table.load does not handle plug-ins that store page tables by
    type

10. AAXTOOL-1234
    The ValidatorRunAllTests DTT script may time out and fail when executing
    a test which takes more than six minutes to complete. For example, the
    script will fail when attempting to run the info.support.host_context
    test on Avid's DownMixer.aaxplugin plug-in on Windows. As a workaround,
    use the --timeout_factor (-x) option when executing the script to
    increase the timeout by some multiplier. For example, -x 10 will change
    the timeout from around seven minutes to around seventy minutes.
