@echo off
set /p dname= Please Enter Release Name (vX.Y.Z_DDMMYYYY): 
md 10_Releases\%dname%

rem copy stuff
xcopy "*.hex" "10_Releases\%dname%\"
xcopy "*.xlsx" "10_Releases\%dname%\"