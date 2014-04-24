@echo off
pushd %~dp0
bcdedit | find "nx"
reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Kernel" /v MitigationOptions 2> nul
for %%f in ( dep_test_nxcompat.exe dep_test_nocompat.exe dep_test_incompat.exe ) do (
echo ^	%%f
%%f
)
REM pause