@echo off
setlocal enabledelayedexpansion 
path %PATH%;%ProgramFiles(x86)%\EMET 5.0;%ProgramFiles%\EMET 5.0;%ProgramFiles(x86)%\EMET 4.1;%ProgramFiles%\EMET 4.1;%PATH%;%ProgramFiles(x86)%\EMET 4.0;%ProgramFiles%\EMET 4.0;%ProgramFiles(x86)%\EMET;%ProgramFiles%\EMET
pushd %~dp0
reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Kernel" /v MitigationOptions 2> nul
reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management" /v MoveImages 2> nul
for %%f in (aslr_*.exe) do (
reg add "HKCU\SOFTWARE\Microsoft\Windows\Windows Error Reporting\ExcludedApplications" /v %%f /t REG_DWORD /d 1 /f > nul 2> nul
echo ^	%%f
%%f
for %%i in (0x100 0x200 0x300 0x500 0x600 0x700) do (
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%%f" /v MitigationOptions /t REG_QWORD /d %%i /f > nul
reg query "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%%f" /v MitigationOptions 2> nul
%%f
if !ERRORLEVEL!==-1073741207 echo Process Creation failed
EMET_Conf.exe --set %%f +SEHOP -MandatoryASLR > nul 2> nul && (
reg query "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%%f" /v MitigationOptions 2> nul
%%f
EMET_Conf.exe --delete %%f > nul
)
reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%%f" /f > nul 2> nul
)
reg delete "HKCU\SOFTWARE\Microsoft\Windows\Windows Error Reporting\ExcludedApplications" /v %%f /f > nul 2> nul
pause
)
popd
endlocal