export
SHELL := /bin/bash

### Linker Script
LDFILE = $(PWD)/stm32l412kb.ld

### Device
DEVICE = STM32L412xx

### Docker
DOCKER_TAG = gcc-arm-none-eabi
DOCKER_RUN = docker run --user $(shell id -u):$(shell id -g) --rm -v $(PWD):$(CURDIR) $(DOCKER_TAG)
DOCKER_FLASH = docker run --rm -v $(PWD):$(CURDIR) --privileged -v /dev/bus/usb:/dev/bus/usb $(DOCKER_TAG)

### Includes
CMSIS_INC_DIR = $(PWD)/CMSIS/Include
CMSIS_ST_INC_DIR = $(PWD)/CMSIS/Device/ST/STM32L4xx/Include

### Sources
CMSIS_SYS_SRC = $(PWD)/CMSIS/Device/ST/STM32L4xx/Source/Templates/system_stm32l4xx.c
CMSIS_START_SRC = $(PWD)/CMSIS/Device/ST/STM32L4xx/Source/Templates/gcc/startup_stm32l412xx.s

CMSIS_OBJS = $(patsubst %.c,%.o,${CMSIS_SYS_SRC}) $(patsubst %.s,%.o,${CMSIS_START_SRC})

### CFLAGS
# 'Standard Options'
CFLAGS = -Wall -Wextra -Wshadow -Wdouble-promotion -O2 -g
# Device Specific
CFLAGS += -D${DEVICE} -mcpu=cortex-m4 -mthumb -T${LDFILE}
# Remove unused sections
CFLAGS += -Wl,--gc-sections  -ffunction-sections -fdata-sections
# FPU
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard

CC = ${DOCKER_RUN} arm-none-eabi-gcc
LD = ${DOCKER_RUN} arm-none-eabi-gcc
AR = ${DOCKER_RUN} arm-none-eabi-ar
AS = ${DOCKER_RUN} arm-none-eabi-as
CP = ${DOCKER_RUN} arm-none-eabi-objcopy
OD = ${DOCKER_RUN} arm-none-eabi-objdump
SZ = ${DOCKER_RUN} arm-none-eabi-size

.PHONY : $(TARGETS) $(SUBDIRS_TARGETS)
TARGETS := all debug clean flash disass size
SUBDIRS := $(wildcard projects/*/.)

# append .target to each subdir
SUBDIRS_TARGETS := \
	$(foreach t,$(TARGETS),$(addsuffix $t,$(SUBDIRS)))

# static pattern rule, expands into:
# all clean : % : dir1/.% dir2/.%
$(TARGETS) : % : $(addsuffix %,$(SUBDIRS))
	@echo 'Done "$*" target'

# here, for foo/.all:
#   $(@D) is foo
#   $(@F) is .all, with leading period
#   $(@F:.%=%) is just all
$(SUBDIRS_TARGETS) :
	$(MAKE) -C $(@D) $(@F:.%=%)

.PHONY: docker-setup
docker-setup:
	@docker build -t $(DOCKER_TAG) $(CURDIR)

.PHONY: format
SRCS := $(shell find projects -type f -name "*.c")
HEADERS = $(shell find projects -type f -name "*.h")
format:
	@for src in $(SRCS) $(HEADERS); do \
		printf "Running format on $$src...\n" ; \
		clang-format \
			-i "$$src"; \
	done

.PHONY: check-format
check-format:
	@for src in $(SRCS) $(HEADERS) $(TEST_SRCS) ; do \
		var=`clang-format "$$src" | diff "$$src" - | wc -l` ; \
		if [ $$var -ne 0 ] ; then \
			echo "$$src does not respect the coding style (diff: $$var lines)" ; \
			exit 1 ; \
		fi ; \
	done
	@printf "Style check passed\n"