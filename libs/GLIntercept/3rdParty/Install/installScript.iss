; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#include "Common.iss"

[Setup]

AppVersion=1.1.0
VersionInfoVersion=1.1.0.0

AppVerName=GLIntercept 1.1.0
DefaultDirName={pf}\GLIntercept_1_1_0
OutputBaseFilename=GLIntercept_1_1_0

AppName=GLIntercept
DefaultGroupName=GLIntercept

[Files]
Source: "..\..\Bin\MainLib\OpenGL32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\Bin\Plugins\*.*"; DestDir: "{app}\Plugins"; Flags: ignoreversion recursesubdirs


