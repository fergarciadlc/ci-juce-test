#define MyAppName "ci-cmake-juce"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Ear Candy Technologies"
#define MyAppURL "https://www.earcandytech.com/"

[Setup]
AppName={#MyAppName}
VersionInfoDescription=Instalador de ci-cmake-juce Plug-in
AppVersion={#MyAppVersion}
VersionInfoVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
VersionInfoCopyright=(C) Ear Candy Technologies
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
CreateAppDir=no
DefaultGroupName={#MyAppName}
OutputDir=.\build
OutputBaseFilename=ci-cmake-juce_1_0_0
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
DisableWelcomePage=no
DisableDirPage=yes

ArchitecturesInstallIn64BitMode=x64
DisableReadyPage=false
LanguageDetectionMethod=uilanguage
VersionInfoCompany={#MyAppPublisher}
VersionInfoProductName={#MyAppPublisher}
VersionInfoProductVersion={#MyAppVersion}
WizardImageStretch=true

[Types]
Name: "full"; Description: "Full installation"
Name: "compact"; Description: "Compact installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "VST364"; Description: "64-bit VST3"; Types: full compact; 
Name: "VST64"; Description: "64-bit VST2"; Types: full;
Name: "AAX"; Description: "64-bit AAX"; Types: full compact;

[Files]
Source: "plugins\ci-cmake-juce.vst3"; DestDir: "{code:GetDir|0}"; Components: VST364; Flags: ignoreversion;
Source: "plugins\ci-cmake-juce.dll"; DestDir: "{code:GetDir|1}"; Components: VST64; Flags: ignoreversion;
Source: "plugins\ci-cmake-juce.aaxplugin"; DestDir: "{code:GetDir|2}"; Components: AAX; Flags: ignoreversion recursesubdirs;

[Code]
var
  DirPage: TInputDirWizardPage;

function GetDir(Param: String): String;
begin
  Result := DirPage.Values[StrToInt(Param)];
end;

procedure InitializeWizard;
begin

  DirPage := CreateInputDirPage(wpSelectComponents,
  'Confirm VST Plugin Directory', '',
  'Select the folder in which setup should install the Plugin, then click Next.',
  False, '');
  
  DirPage.Add('VST3 Plug-in');
  DirPage.Add('VST Plug-in');
  DirPage.Add('AAX Plug-in');

  DirPage.Values[0] := GetPreviousData('Directory1', ExpandConstant('{cf64}\VST3'));
  DirPage.Values[1] := GetPreviousData('Directory2', ExpandConstant('{pf64}\Steinberg\VstPlugins'));
  DirPage.Values[2] := GetPreviousData('Directory3', ExpandConstant('{cf64}\Avid\Audio\Plug-Ins'));
end;

procedure RegisterPreviousData(PreviousDataKey: Integer);
begin
  SetPreviousData(PreviousDataKey, 'Directory1', DirPage.Values[0]);
  SetPreviousData(PreviousDataKey, 'Directory2', DirPage.Values[1]);
  SetPreviousData(PreviousDataKey, 'Directory3', DirPage.Values[2]);
end;