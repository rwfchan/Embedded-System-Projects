################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
gpio_if.obj: D:/TI/CC3200SDK_1.5.0/cc3200-sdk/example/common/gpio_if.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/TI/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/TI/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/driverlib" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/inc" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/example/common" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/source" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/include" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink_extlib/provisioninglib" --define=ccs --define=cc3200 -g --gcc --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="gpio_if.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/TI/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/TI/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/driverlib" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/inc" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/example/common" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/source" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/include" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink_extlib/provisioninglib" --define=ccs --define=cc3200 -g --gcc --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

network_common.obj: D:/TI/CC3200SDK_1.5.0/cc3200-sdk/example/common/network_common.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/TI/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/TI/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/driverlib" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/inc" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/example/common" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/source" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/include" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink_extlib/provisioninglib" --define=ccs --define=cc3200 -g --gcc --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="network_common.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_ccs.obj: D:/TI/CC3200SDK_1.5.0/cc3200-sdk/example/common/startup_ccs.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/TI/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/TI/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/driverlib" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/inc" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/example/common" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/source" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink/include" --include_path="D:/TI/CC3200SDK_1.5.0/cc3200-sdk/simplelink_extlib/provisioninglib" --define=ccs --define=cc3200 -g --gcc --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="startup_ccs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


