@echo off
set /p dname= Please Enter Project Name: 

rem copy stuff
xcopy "10_Template\10_NewProjectTemplate\Source\c\04_App\app.c" "30_Projects\%dname%\Source\c\04_App\" 
xcopy "10_Template\10_NewProjectTemplate\Source\h\04_App\app.h" "30_Projects\%dname%\Source\h\04_App\" 
xcopy "10_Template\10_NewProjectTemplate\Source\h\04_App\sys_cfg.h" "30_Projects\%dname%\Source\h\04_App\" 
xcopy /s/y "10_Template\10_NewProjectTemplate\AppTemplate.X\nbproject" "30_Projects\%dname%\AppTemplate.X\nbproject\" /E
xcopy "10_Template\10_NewProjectTemplate\AppTemplate.X\Makefile" "30_Projects\%dname%\AppTemplate.X\"
