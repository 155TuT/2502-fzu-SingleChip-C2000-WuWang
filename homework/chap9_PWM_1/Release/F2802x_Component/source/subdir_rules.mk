################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
F2802x_Component/source/%.obj: ../F2802x_Component/source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt -O2 --include_path="C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="C:/Users/17169/Documents/Mycode/fzucourses/Electivecourse/2502-fzu-SingleChip-C2000-WuWang/homework/chap9_PWM_1" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="F2802x_Component/source/$(basename $(<F)).d_raw" --obj_directory="F2802x_Component/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2802x_Component/source/%.obj: ../F2802x_Component/source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt -O2 --include_path="C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="C:/Users/17169/Documents/Mycode/fzucourses/Electivecourse/2502-fzu-SingleChip-C2000-WuWang/homework/chap9_PWM_1" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="F2802x_Component/source/$(basename $(<F)).d_raw" --obj_directory="F2802x_Component/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


