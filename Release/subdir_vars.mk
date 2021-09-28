################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../main_file.cfg 

CMD_SRCS += \
../F2802x_Headers_BIOS.cmd \
../TMS320F28027.cmd 

ASM_SRCS += \
../CodeStartBranch.asm 

C_SRCS += \
../DeviceInit_18Nov2018.c \
../F2802x_GlobalVariableDefs.c \
../main_file.c \
../spi_screen.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./DeviceInit_18Nov2018.d \
./F2802x_GlobalVariableDefs.d \
./main_file.d \
./spi_screen.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./CodeStartBranch.obj \
./DeviceInit_18Nov2018.obj \
./F2802x_GlobalVariableDefs.obj \
./main_file.obj \
./spi_screen.obj 

ASM_DEPS += \
./CodeStartBranch.d 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"CodeStartBranch.obj" \
"DeviceInit_18Nov2018.obj" \
"F2802x_GlobalVariableDefs.obj" \
"main_file.obj" \
"spi_screen.obj" 

C_DEPS__QUOTED += \
"DeviceInit_18Nov2018.d" \
"F2802x_GlobalVariableDefs.d" \
"main_file.d" \
"spi_screen.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

ASM_DEPS__QUOTED += \
"CodeStartBranch.d" 

ASM_SRCS__QUOTED += \
"../CodeStartBranch.asm" 

C_SRCS__QUOTED += \
"../DeviceInit_18Nov2018.c" \
"../F2802x_GlobalVariableDefs.c" \
"../main_file.c" \
"../spi_screen.c" 


