#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=--mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../Framework/MercuryFwk/Source/c/00_Main/main.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/adc_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/i2c_slv_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/lcd_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/led_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/mcu_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/sonar_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/tmr_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/servo_drv.c ../../../Framework/MercuryFwk/Source/c/02_Osl/interrupts.c ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched.c ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched_tbl.c ../../../Framework/MercuryFwk/Source/c/02_Osl/os_ser.c ../../../Framework/MercuryFwk/Source/c/03_Sml/01_Pml/01_SlvI2c/i2c_slv.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/led.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/sf.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/std_ser.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/test.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/wdg.c ../Source/c/04_App/sys_init.c ../Source/c/04_App/sb130_app.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1915165344/main.p1 ${OBJECTDIR}/_ext/1598173325/adc_drv.p1 ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1 ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1 ${OBJECTDIR}/_ext/1598173325/led_drv.p1 ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1 ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1 ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1 ${OBJECTDIR}/_ext/1598173325/servo_drv.p1 ${OBJECTDIR}/_ext/771353987/interrupts.p1 ${OBJECTDIR}/_ext/771353987/os_sched.p1 ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1 ${OBJECTDIR}/_ext/771353987/os_ser.p1 ${OBJECTDIR}/_ext/675308290/i2c_slv.p1 ${OBJECTDIR}/_ext/519912432/led.p1 ${OBJECTDIR}/_ext/519912432/sf.p1 ${OBJECTDIR}/_ext/519912432/std_ser.p1 ${OBJECTDIR}/_ext/519912432/test.p1 ${OBJECTDIR}/_ext/519912432/wdg.p1 ${OBJECTDIR}/_ext/538292599/sys_init.p1 ${OBJECTDIR}/_ext/538292599/sb130_app.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1915165344/main.p1.d ${OBJECTDIR}/_ext/1598173325/adc_drv.p1.d ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1.d ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1.d ${OBJECTDIR}/_ext/1598173325/led_drv.p1.d ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1.d ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1.d ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1.d ${OBJECTDIR}/_ext/1598173325/servo_drv.p1.d ${OBJECTDIR}/_ext/771353987/interrupts.p1.d ${OBJECTDIR}/_ext/771353987/os_sched.p1.d ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1.d ${OBJECTDIR}/_ext/771353987/os_ser.p1.d ${OBJECTDIR}/_ext/675308290/i2c_slv.p1.d ${OBJECTDIR}/_ext/519912432/led.p1.d ${OBJECTDIR}/_ext/519912432/sf.p1.d ${OBJECTDIR}/_ext/519912432/std_ser.p1.d ${OBJECTDIR}/_ext/519912432/test.p1.d ${OBJECTDIR}/_ext/519912432/wdg.p1.d ${OBJECTDIR}/_ext/538292599/sys_init.p1.d ${OBJECTDIR}/_ext/538292599/sb130_app.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1915165344/main.p1 ${OBJECTDIR}/_ext/1598173325/adc_drv.p1 ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1 ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1 ${OBJECTDIR}/_ext/1598173325/led_drv.p1 ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1 ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1 ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1 ${OBJECTDIR}/_ext/1598173325/servo_drv.p1 ${OBJECTDIR}/_ext/771353987/interrupts.p1 ${OBJECTDIR}/_ext/771353987/os_sched.p1 ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1 ${OBJECTDIR}/_ext/771353987/os_ser.p1 ${OBJECTDIR}/_ext/675308290/i2c_slv.p1 ${OBJECTDIR}/_ext/519912432/led.p1 ${OBJECTDIR}/_ext/519912432/sf.p1 ${OBJECTDIR}/_ext/519912432/std_ser.p1 ${OBJECTDIR}/_ext/519912432/test.p1 ${OBJECTDIR}/_ext/519912432/wdg.p1 ${OBJECTDIR}/_ext/538292599/sys_init.p1 ${OBJECTDIR}/_ext/538292599/sb130_app.p1

# Source Files
SOURCEFILES=../../../Framework/MercuryFwk/Source/c/00_Main/main.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/adc_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/i2c_slv_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/lcd_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/led_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/mcu_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/sonar_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/tmr_drv.c ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/servo_drv.c ../../../Framework/MercuryFwk/Source/c/02_Osl/interrupts.c ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched.c ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched_tbl.c ../../../Framework/MercuryFwk/Source/c/02_Osl/os_ser.c ../../../Framework/MercuryFwk/Source/c/03_Sml/01_Pml/01_SlvI2c/i2c_slv.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/led.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/sf.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/std_ser.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/test.c ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/wdg.c ../Source/c/04_App/sys_init.c ../Source/c/04_App/sb130_app.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F1829
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1915165344/main.p1: ../../../Framework/MercuryFwk/Source/c/00_Main/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1915165344" 
	@${RM} ${OBJECTDIR}/_ext/1915165344/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1915165344/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1915165344/main.p1 ../../../Framework/MercuryFwk/Source/c/00_Main/main.c 
	@-${MV} ${OBJECTDIR}/_ext/1915165344/main.d ${OBJECTDIR}/_ext/1915165344/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1915165344/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/adc_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/adc_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/adc_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/adc_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/adc_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/adc_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/adc_drv.d ${OBJECTDIR}/_ext/1598173325/adc_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/adc_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/i2c_slv_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/i2c_slv_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.d ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/lcd_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/lcd_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/lcd_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/lcd_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/lcd_drv.d ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/led_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/led_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/led_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/led_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/led_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/led_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/led_drv.d ${OBJECTDIR}/_ext/1598173325/led_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/led_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/mcu_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/mcu_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/mcu_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/mcu_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/mcu_drv.d ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/sonar_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/sonar_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/sonar_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/sonar_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/sonar_drv.d ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/tmr_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/tmr_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/tmr_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/tmr_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/tmr_drv.d ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/servo_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/servo_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/servo_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/servo_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/servo_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/servo_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/servo_drv.d ${OBJECTDIR}/_ext/1598173325/servo_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/servo_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/771353987/interrupts.p1: ../../../Framework/MercuryFwk/Source/c/02_Osl/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/771353987" 
	@${RM} ${OBJECTDIR}/_ext/771353987/interrupts.p1.d 
	@${RM} ${OBJECTDIR}/_ext/771353987/interrupts.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/771353987/interrupts.p1 ../../../Framework/MercuryFwk/Source/c/02_Osl/interrupts.c 
	@-${MV} ${OBJECTDIR}/_ext/771353987/interrupts.d ${OBJECTDIR}/_ext/771353987/interrupts.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/771353987/interrupts.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/771353987/os_sched.p1: ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/771353987" 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_sched.p1.d 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_sched.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/771353987/os_sched.p1 ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched.c 
	@-${MV} ${OBJECTDIR}/_ext/771353987/os_sched.d ${OBJECTDIR}/_ext/771353987/os_sched.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/771353987/os_sched.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1: ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched_tbl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/771353987" 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1.d 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1 ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched_tbl.c 
	@-${MV} ${OBJECTDIR}/_ext/771353987/os_sched_tbl.d ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/771353987/os_ser.p1: ../../../Framework/MercuryFwk/Source/c/02_Osl/os_ser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/771353987" 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_ser.p1.d 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_ser.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/771353987/os_ser.p1 ../../../Framework/MercuryFwk/Source/c/02_Osl/os_ser.c 
	@-${MV} ${OBJECTDIR}/_ext/771353987/os_ser.d ${OBJECTDIR}/_ext/771353987/os_ser.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/771353987/os_ser.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/675308290/i2c_slv.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/01_Pml/01_SlvI2c/i2c_slv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/675308290" 
	@${RM} ${OBJECTDIR}/_ext/675308290/i2c_slv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/675308290/i2c_slv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/675308290/i2c_slv.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/01_Pml/01_SlvI2c/i2c_slv.c 
	@-${MV} ${OBJECTDIR}/_ext/675308290/i2c_slv.d ${OBJECTDIR}/_ext/675308290/i2c_slv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/675308290/i2c_slv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/led.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/led.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/led.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/led.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/led.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/led.d ${OBJECTDIR}/_ext/519912432/led.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/led.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/sf.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/sf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/sf.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/sf.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/sf.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/sf.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/sf.d ${OBJECTDIR}/_ext/519912432/sf.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/sf.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/std_ser.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/std_ser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/std_ser.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/std_ser.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/std_ser.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/std_ser.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/std_ser.d ${OBJECTDIR}/_ext/519912432/std_ser.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/std_ser.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/test.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/test.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/test.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/test.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/test.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/test.d ${OBJECTDIR}/_ext/519912432/test.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/test.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/wdg.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/wdg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/wdg.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/wdg.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/wdg.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/wdg.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/wdg.d ${OBJECTDIR}/_ext/519912432/wdg.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/wdg.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/538292599/sys_init.p1: ../Source/c/04_App/sys_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/538292599" 
	@${RM} ${OBJECTDIR}/_ext/538292599/sys_init.p1.d 
	@${RM} ${OBJECTDIR}/_ext/538292599/sys_init.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/538292599/sys_init.p1 ../Source/c/04_App/sys_init.c 
	@-${MV} ${OBJECTDIR}/_ext/538292599/sys_init.d ${OBJECTDIR}/_ext/538292599/sys_init.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/538292599/sys_init.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/538292599/sb130_app.p1: ../Source/c/04_App/sb130_app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/538292599" 
	@${RM} ${OBJECTDIR}/_ext/538292599/sb130_app.p1.d 
	@${RM} ${OBJECTDIR}/_ext/538292599/sb130_app.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=icd3    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/538292599/sb130_app.p1 ../Source/c/04_App/sb130_app.c 
	@-${MV} ${OBJECTDIR}/_ext/538292599/sb130_app.d ${OBJECTDIR}/_ext/538292599/sb130_app.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/538292599/sb130_app.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1915165344/main.p1: ../../../Framework/MercuryFwk/Source/c/00_Main/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1915165344" 
	@${RM} ${OBJECTDIR}/_ext/1915165344/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1915165344/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1915165344/main.p1 ../../../Framework/MercuryFwk/Source/c/00_Main/main.c 
	@-${MV} ${OBJECTDIR}/_ext/1915165344/main.d ${OBJECTDIR}/_ext/1915165344/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1915165344/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/adc_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/adc_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/adc_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/adc_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/adc_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/adc_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/adc_drv.d ${OBJECTDIR}/_ext/1598173325/adc_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/adc_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/i2c_slv_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/i2c_slv_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.d ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/i2c_slv_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/lcd_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/lcd_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/lcd_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/lcd_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/lcd_drv.d ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/lcd_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/led_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/led_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/led_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/led_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/led_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/led_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/led_drv.d ${OBJECTDIR}/_ext/1598173325/led_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/led_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/mcu_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/mcu_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/mcu_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/mcu_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/mcu_drv.d ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/mcu_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/sonar_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/sonar_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/sonar_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/sonar_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/sonar_drv.d ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/sonar_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/tmr_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/tmr_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/tmr_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/tmr_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/tmr_drv.d ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/tmr_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1598173325/servo_drv.p1: ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/servo_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1598173325" 
	@${RM} ${OBJECTDIR}/_ext/1598173325/servo_drv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1598173325/servo_drv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1598173325/servo_drv.p1 ../../../Framework/MercuryFwk/Source/c/01_Hal/02_Drv/servo_drv.c 
	@-${MV} ${OBJECTDIR}/_ext/1598173325/servo_drv.d ${OBJECTDIR}/_ext/1598173325/servo_drv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1598173325/servo_drv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/771353987/interrupts.p1: ../../../Framework/MercuryFwk/Source/c/02_Osl/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/771353987" 
	@${RM} ${OBJECTDIR}/_ext/771353987/interrupts.p1.d 
	@${RM} ${OBJECTDIR}/_ext/771353987/interrupts.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/771353987/interrupts.p1 ../../../Framework/MercuryFwk/Source/c/02_Osl/interrupts.c 
	@-${MV} ${OBJECTDIR}/_ext/771353987/interrupts.d ${OBJECTDIR}/_ext/771353987/interrupts.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/771353987/interrupts.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/771353987/os_sched.p1: ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/771353987" 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_sched.p1.d 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_sched.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/771353987/os_sched.p1 ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched.c 
	@-${MV} ${OBJECTDIR}/_ext/771353987/os_sched.d ${OBJECTDIR}/_ext/771353987/os_sched.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/771353987/os_sched.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1: ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched_tbl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/771353987" 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1.d 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1 ../../../Framework/MercuryFwk/Source/c/02_Osl/os_sched_tbl.c 
	@-${MV} ${OBJECTDIR}/_ext/771353987/os_sched_tbl.d ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/771353987/os_sched_tbl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/771353987/os_ser.p1: ../../../Framework/MercuryFwk/Source/c/02_Osl/os_ser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/771353987" 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_ser.p1.d 
	@${RM} ${OBJECTDIR}/_ext/771353987/os_ser.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/771353987/os_ser.p1 ../../../Framework/MercuryFwk/Source/c/02_Osl/os_ser.c 
	@-${MV} ${OBJECTDIR}/_ext/771353987/os_ser.d ${OBJECTDIR}/_ext/771353987/os_ser.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/771353987/os_ser.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/675308290/i2c_slv.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/01_Pml/01_SlvI2c/i2c_slv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/675308290" 
	@${RM} ${OBJECTDIR}/_ext/675308290/i2c_slv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/675308290/i2c_slv.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/675308290/i2c_slv.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/01_Pml/01_SlvI2c/i2c_slv.c 
	@-${MV} ${OBJECTDIR}/_ext/675308290/i2c_slv.d ${OBJECTDIR}/_ext/675308290/i2c_slv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/675308290/i2c_slv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/led.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/led.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/led.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/led.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/led.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/led.d ${OBJECTDIR}/_ext/519912432/led.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/led.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/sf.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/sf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/sf.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/sf.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/sf.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/sf.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/sf.d ${OBJECTDIR}/_ext/519912432/sf.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/sf.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/std_ser.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/std_ser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/std_ser.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/std_ser.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/std_ser.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/std_ser.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/std_ser.d ${OBJECTDIR}/_ext/519912432/std_ser.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/std_ser.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/test.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/test.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/test.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/test.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/test.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/test.d ${OBJECTDIR}/_ext/519912432/test.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/test.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/519912432/wdg.p1: ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/wdg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/519912432" 
	@${RM} ${OBJECTDIR}/_ext/519912432/wdg.p1.d 
	@${RM} ${OBJECTDIR}/_ext/519912432/wdg.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/519912432/wdg.p1 ../../../Framework/MercuryFwk/Source/c/03_Sml/02_Ssl/wdg.c 
	@-${MV} ${OBJECTDIR}/_ext/519912432/wdg.d ${OBJECTDIR}/_ext/519912432/wdg.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/519912432/wdg.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/538292599/sys_init.p1: ../Source/c/04_App/sys_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/538292599" 
	@${RM} ${OBJECTDIR}/_ext/538292599/sys_init.p1.d 
	@${RM} ${OBJECTDIR}/_ext/538292599/sys_init.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/538292599/sys_init.p1 ../Source/c/04_App/sys_init.c 
	@-${MV} ${OBJECTDIR}/_ext/538292599/sys_init.d ${OBJECTDIR}/_ext/538292599/sys_init.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/538292599/sys_init.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/538292599/sb130_app.p1: ../Source/c/04_App/sb130_app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/538292599" 
	@${RM} ${OBJECTDIR}/_ext/538292599/sb130_app.p1.d 
	@${RM} ${OBJECTDIR}/_ext/538292599/sb130_app.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/538292599/sb130_app.p1 ../Source/c/04_App/sb130_app.c 
	@-${MV} ${OBJECTDIR}/_ext/538292599/sb130_app.d ${OBJECTDIR}/_ext/538292599/sb130_app.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/538292599/sb130_app.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.map  -D__DEBUG=1  --debugger=icd3  -DXPRJ_default=$(CND_CONF)    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"        $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)    --double=24 --float=24 --opt=+asm,+asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=free -P -N255 -I"../../../Framework/MercuryFwk/Source/h/01_Hal/01_Sys" -I"../../../Framework/MercuryFwk/Source/h/01_Hal/02_Drv" -I"../../../Framework/MercuryFwk/Source/h/02_Osl" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/01_SlvI2c" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/01_Pml/02_SlvSer" -I"../../../Framework/MercuryFwk/Source/h/03_Sml/02_Ssl" -I"../Source/h/04_App" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/SB130_Servo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
