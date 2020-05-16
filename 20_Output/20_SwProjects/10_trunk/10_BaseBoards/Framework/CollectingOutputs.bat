@echo removed directory if present
rmdir /s /q "MSF"

@echo Framework
xcopy "MercuryFwk\Source" "MSF\MercuryFwk\Source\" /E

@echo Tools
xcopy "..\Bootloader\BootHostApp" "MSF\Tools\Bootloader\" /E
xcopy "FwkRelease\20_Miscellaneous\30_UsbDrivers" "MSF\Tools\USBDrivers\" /E

@echo Slave Update Package
xcopy "..\..\..\..\30_Documentation\20_UserManuals\40_SlaveFwUpgradePackage\MS_SlaveFwUpgradePackage.pdf" "MSF\Tools\SlaveUpdatePackage\"
xcopy "..\..\20_SlaveBoards\Boards\01_FwImages\*.hex" "MSF\Tools\SlaveUpdatePackage\FwImages\LastVersion\"
xcopy "..\..\20_SlaveBoards\Boards\01_FwImages\10_Releases\*.rar" "MSF\Tools\SlaveUpdatePackage\FwImages\Archive\"
xcopy "..\..\20_SlaveBoards\Boards\01_FwImages\*.xlsx" "MSF\Tools\SlaveUpdatePackage\FwImages\"

@echo App Template
xcopy "FwkRelease\10_Template\10_NewProjectTemplate\Source" "MSF\MercuryProjects\10_Template\10_NewProjectTemplate\Source\" /E
xcopy "FwkRelease\10_Template\10_NewProjectTemplate\AppTemplate.X\nbproject" "MSF\MercuryProjects\10_Template\10_NewProjectTemplate\AppTemplate.X\nbproject\" /E
xcopy /s/y "FwkRelease\10_Template\10_NewProjectTemplate\AppTemplate.X\Makefile" "MSF\MercuryProjects\10_Template\10_NewProjectTemplate\AppTemplate.X\"
xcopy "FwkRelease\CreateNewProject.bat" "MSF\MercuryProjects\"

@echo Demo
xcopy "FwkRelease\20_Examples" "MSF\MercuryProjects\20_Examples\" /E

@echo Project Folder
md MSF\MercuryProjects\30_Projects

@echo Precompiled Demo
rem not used at the moment

@echo Datasheets
xcopy "..\..\..\..\30_Documentation\10_Datasheet\BB110_BaseBoardPic18F\MS_BB110_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\EB110_ExpansionDual\MS_EB110_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\EB111_ExpansionQuad\MS_EB111_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\EB210_ExpansionLCD16x2\MS_EB210_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\MB210_WifiModemBoard\MS_MB210_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\MB310_BtModemBoard\MS_MB310_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\SB110_RelayBoard\MS_SB110_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\SB120_NeopixelBoard\MS_SB120_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\SB130_ServoBoard\MS_SB130_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\SB140_HSDBoard\MS_SB140_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\SB310_UltrasonicBoard\MS_SB310_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\SB320_InfraredBoard\MS_SB320_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\SB330_TemperatureBoard\MS_SB330_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\SB810_ProtoBoardTypeA\MS_SB810_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\SB111_BigRelayBoard\MS_SB111_Datasheet.pdf" "MSF\Documentation\Datasheets\"
xcopy "..\..\..\..\30_Documentation\10_Datasheet\BL810_BrainlessProto\MS_BL810_Datasheet.pdf" "MSF\Documentation\Datasheets\"

@echo Manuals
xcopy "..\..\..\..\30_Documentation\20_UserManuals\10_GettingStartedGuide\*.pdf" "MSF\Documentation\Manuals\"
xcopy "..\..\..\..\30_Documentation\20_UserManuals\20_FrameworkUserManual\*.pdf" "MSF\Documentation\Manuals\"
xcopy "..\..\..\..\30_Documentation\20_UserManuals\30_SlavesGenericCommandSet\*.pdf" "MSF\Documentation\Manuals\"

@echo AN
xcopy "..\..\..\..\30_Documentation\30_ApplicationNotes\MS_AN_001\*.pdf" "MSF\Documentation\ApplicationNotes\"
xcopy "..\..\..\..\30_Documentation\30_ApplicationNotes\MS_AN_002\*.pdf" "MSF\Documentation\ApplicationNotes\"

@echo RN
xcopy "..\..\..\..\30_Documentation\50_ReleaseNotes\*.pdf" "MSF\Documentation\ReleaseNotes\"

@echo delete useless files
del "MSF\MercuryFwk\Source\c\03_Sml\02_Ssl\test.c"
del "MSF\MercuryFwk\Source\h\03_Sml\02_Ssl\test.h"
rmdir /s /q "MSF\MercuryFwk\Source\h\00_Cfg"