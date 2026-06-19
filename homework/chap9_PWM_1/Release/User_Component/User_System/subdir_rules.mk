################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
User_Component/User_System/%.obj: ../User_Component/User_System/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt -O2 --include_path="C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="C:/Users/17169/Documents/Mycode/fzucourses/Electivecourse/2502-fzu-SingleChip-C2000-WuWang/homework/chap9_PWM_1" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="User_Component/User_System/$(basename $(<F)).d_raw" --obj_directory="User_Component/User_System" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


