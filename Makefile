BUILD_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


SPL_PATH = $(BUILD_ROOT)/stm_spl/
COMMON_PATH = $(BUILD_ROOT)/common
LABS_DIR = $(BUILD_ROOT)/projects

.PHONY: labs spl common lab1 clean

all: labs

labs: lab1 lab2 lab3 lab4 lab5

spl:
	make -C $(SPL_PATH)

common:
	make -C $(COMMON_PATH)

lab1: spl common
	make -C $(LABS_DIR)/task_1_gpio_basics 
	
lab2: spl common
	make -C $(LABS_DIR)/task_2_timers_basics
	
lab3: spl common
	make -C $(LABS_DIR)/task_3_timers_interrupts
	
lab4: spl common
	make -C $(LABS_DIR)/task_4_pwm
	
lab5: spl common
	make -C $(LABS_DIR)/task_5_library_composition
	
clean:
	make -C $(SPL_PATH) clean
	make -C $(COMMON_PATH) clean
	make -C $(LABS_DIR)/task_1_gpio_basics  clean
	make -C $(LABS_DIR)/task_2_timers_basics clean
	make -C $(LABS_DIR)/task_3_timers_interrupts clean
	make -C $(LABS_DIR)/task_4_pwm  clean
	make -C $(LABS_DIR)/task_5_library_composition clean
