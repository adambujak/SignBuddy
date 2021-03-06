TARGET = SignBuddy
BUILD_CONFIG ?= DEBUG
BOARD_VERSION ?= BUDDY


#Paths
BUILD_DIR = _build
FIRMWARE_ROOT = ../..

SDK_ROOT = $(FIRMWARE_ROOT)/dependencies/STM32CubeL0
HAL_ROOT = $(SDK_ROOT)/Drivers/STM32L0xx_HAL_Driver

FREERTOS_ROOT = $(FIRMWARE_ROOT)/dependencies/freertos_kernel
FREERTOS_PORTABLE = $(FREERTOS_ROOT)/portable/GCC/ARM_CM0

LIB_ROOT = $(FIRMWARE_ROOT)/libraries
PROJ_ROOT = $(FIRMWARE_ROOT)/applications/SignBuddy

NANOPB_ROOT = $(FIRMWARE_ROOT)/dependencies/nanopb

PB_SOURCES = \
	$(NANOPB_ROOT)/pb_common.c \
	$(NANOPB_ROOT)/pb_encode.c \

PROTO_SOURCE = $(PROJ_ROOT)/sign_buddy.pb.c

FREERTOS_SOURCES = \
	$(FREERTOS_ROOT)/tasks.c \
	$(FREERTOS_ROOT)/queue.c \
	$(FREERTOS_ROOT)/list.c \
	$(FREERTOS_ROOT)/timers.c \
	$(FREERTOS_ROOT)/event_groups.c \
	$(FREERTOS_ROOT)/stream_buffer.c \
	$(FREERTOS_ROOT)/portable/MemMang/heap_3.c \
	$(FREERTOS_PORTABLE)/port.c \

HAL_SOURCES = \
	$(HAL_ROOT)/Src/stm32l0xx_ll_adc.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_gpio.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_i2c.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_lpuart.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_rcc.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_tim.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_utils.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_usart.c \
	$(HAL_ROOT)/Src/stm32l0xx_hal.c \
	$(HAL_ROOT)/Src/stm32l0xx_hal_cortex.c \
	$(HAL_ROOT)/Src/stm32l0xx_hal_rcc.c \
	$(HAL_ROOT)/Src/stm32l0xx_hal_tsc.c \


LIB_SOURCES = \
	$(LIB_ROOT)/bno055/bno055.c \
	$(LIB_ROOT)/fast_fifo/fifo.c \

C_SOURCES = \
	main.c \
	adc.c \
	ble_uart.c \
	comms.c \
	crc.c \
	dma.c \
	flex.c \
	gpio.c \
	i2c.c \
	imu.c \
	log_uart.c \
	logger.c \
	sensors.c \
	system_stm32l0xx.c \
	tsc.c \
	$(PROTO_SOURCE) \
	$(LIB_SOURCES) \
	$(HAL_SOURCES) \
	$(FREERTOS_SOURCES) \
	$(PB_SOURCES) \

C_INCLUDES = \
	-I$(SDK_ROOT)/Drivers/CMSIS/Device/ST/STM32L0xx/Include \
	-I$(SDK_ROOT)/Drivers/CMSIS/Include \
	-I$(HAL_ROOT)/Inc \
	-I$(PROJ_ROOT) \
	-I$(LIB_ROOT)/fast_fifo \
	-I$(LIB_ROOT)/bno055 \
	-I$(NANOPB_ROOT) \
	-I$(FREERTOS_ROOT)/include \
	-I$(FREERTOS_PORTABLE) \

STARTUP_FILE = startup_stm32l053xx.s

ASM_SOURCES = \
	$(STARTUP_FILE)

AS_INCLUDES = 


PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m0plus

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

ifeq ($(BOARD_VERSION), NUCLEO)
CHIPSET_DEF = STM32L053xx
BOARD_DEF = BOARD_NUCLEO
endif

ifeq ($(BOARD_VERSION), BUDDY)
CHIPSET_DEF = STM32L053xx
BOARD_DEF = BOARD_BUDDY
endif

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS +=  \
	-D$(CHIPSET_DEF) \
	-D$(BOARD_DEF) \
	-DUSE_HAL_DRIVER \
	-DUSE_FULL_LL_DRIVER \
	-DUSE_FULL_ASSERT \


OPT = -O0 -g3

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32L053R8Tx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo compiling $<
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo assembling $<
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo linking $<
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	

$(BUILD_DIR):
	mkdir $@		

FLASH_SCRIPT = $(realpath $(PROJ_ROOT))/flash_stm.jlink

flash: all
	# retry st-flash on failure
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000 || st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000
	st-flash reset

flash_jlink:
	JLinkExe -device STM32L053R8 -CommandFile $(FLASH_SCRIPT)

clean:
	-rm -fR $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/*.d)

