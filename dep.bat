@echo off
pushd %~dp0
bcdedit | find "nx"
reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Kernel" /v MitigationOptions 2> nul
for %%f in ( dep_test_nxcompat.exe dep_test_nocompat.exe dep_test_incompat.exe ) do (
echo ^	%%f
reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%%f" /f 2> nul
%%f
pause
for %%g in ( 0 1 2 3 4 5 6 7 0x10 0x20 0x30 0x40 0x50 0x60 0x70 ) do (
echo ^	 %%g
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%%f" /v MitigationOptions /t REG_QWORD /d %%g /f > nul
%%f
reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\%%f" /f 2> nul
pause
)
echo.
)