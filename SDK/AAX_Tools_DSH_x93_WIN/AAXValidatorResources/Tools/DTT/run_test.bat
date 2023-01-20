GOTO EndCopyright
 *
 * Copyright 2014 by Avid Technology, Inc.
 *
 :EndCopyright

@echo off

set CUSTOMRUBY=%~dp0%..\ruby_dist\bin\ruby.exe
set SYSTEMRUBY=ruby

if exist "%CUSTOMRUBY%" (
	set RUBYPATH=%CUSTOMRUBY%
) else (
	set RUBYPATH=%SYSTEMRUBY%
)

set DEVSUITECMD=%~dp0%..\sources\bin\runsuite.rb
set BUILTSUITECMD=%~dp0%sources\bin\runsuite.rb

if exist "%DEVSUITECMD%" (
	set RUNSUITE=%DEVSUITECMD%
) else (
	set RUNSUITE=%BUILTSUITECMD%
)


call "%RUBYPATH%" "%RUNSUITE%" %*