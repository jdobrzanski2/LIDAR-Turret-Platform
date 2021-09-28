################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti_v8/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt -O2 --include_path="C:/Users/Joseph/Desktop/Romalo_project/screen_w_BIOS" --include_path="C:/ti_v8/bios_6_76_03_01/packages/ti/posix/ccs" --include_path="C:/ti_v8/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti_v8/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt -O2 --include_path="C:/Users/Joseph/Desktop/Romalo_project/screen_w_BIOS" --include_path="C:/ti_v8/bios_6_76_03_01/packages/ti/posix/ccs" --include_path="C:/ti_v8/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-181467663:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-181467663-inproc

build-181467663-inproc: ../main_file.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti_v8/xdctools_3_51_01_18_core/xs" --xdcpath="C:/ti_v8/bios_6_76_03_01/packages;" xdc.tools.configuro -o configPkg -t ti.targets.C28_large -p ti.platforms.tms320x28:TMS320F28027 -r release -c "C:/ti_v8/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS" --compileOptions "-g" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-181467663 ../main_file.cfg
configPkg/compiler.opt: build-181467663
configPkg/: build-181467663


