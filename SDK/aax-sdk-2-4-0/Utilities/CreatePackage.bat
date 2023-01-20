REM        Copyright 2017, 2020-2021 by Avid Technology, Inc.
set OutDir="%~fn1"
set IconSource="%~fn2"

echo Create Package Directories

IF EXIST %OutDir% GOTO OUTDIR_EXISTS
mkdir %OutDir%
:OUTDIR_EXISTS

IF EXIST %OutDir%\..\Win32 GOTO Win32_EXISTS
mkdir %OutDir%\..\Win32
:Win32_EXISTS

IF EXIST %OutDir%\..\x64 GOTO X64_EXISTS
mkdir %OutDir%\..\x64
:X64_EXISTS

IF EXIST %OutDir%\..\Resources GOTO RESOURCES_EXISTS
mkdir %OutDir%\..\Resources
:RESOURCES_EXISTS

echo Set Folder Icon

IF EXIST %OutDir%\..\..\PlugIn.ico GOTO ICON_EXISTS
copy /Y %IconSource% %OutDir%\..\..\PlugIn.ico > NUL
:ICON_EXISTS

attrib -r %OutDir%\..\..
attrib -h -r -s %OutDir%\..\..\desktop.ini
echo [.ShellClassInfo] > %OutDir%\..\..\desktop.ini 
echo IconResource=PlugIn.ico,0 >> %OutDir%\..\..\desktop.ini 
echo ;For compatibility with Windows XP >> %OutDir%\..\..\desktop.ini 
echo IconFile=PlugIn.ico >> %OutDir%\..\..\desktop.ini 
echo IconIndex=0 >> %OutDir%\..\..\desktop.ini 
attrib +h +r +s %OutDir%\..\..\PlugIn.ico
attrib +h +r +s %OutDir%\..\..\desktop.ini
attrib %OutDir%\..\..
