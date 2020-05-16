@echo off
set /p dname= Please Enter Project Name: 

rem copy stuff
xcopy "10_Template\10_NewProjectTemplate\Source\c\04_App\app.c" "20_Examples\%dname%\Source\c\04_App\" 
xcopy "10_Template\10_NewProjectTemplate\Source\h\04_App\app.h" "20_Examples\%dname%\Source\h\04_App\" 
xcopy "10_Template\10_NewProjectTemplate\Source\h\04_App\sys_cfg.h" "20_Examples\%dname%\Source\h\04_App\" 
xcopy /s/y "10_Template\10_NewProjectTemplate\AppTemplate.X\nbproject" "20_Examples\%dname%\AppTemplate.X\nbproject\" /E
xcopy "10_Template\10_NewProjectTemplate\AppTemplate.X\Makefile" "20_Examples\%dname%\AppTemplate.X\"
