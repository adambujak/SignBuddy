TARGET = SignBuddy
BUILD_CONFIG ?= DEBUG
BOARD_VERSION ?= DISCO_L4


#Paths
BUILD_DIR = _build
FIRMWARE_ROOT = ../..

ifeq ($(BOARD_VERSION), DISCO_L4)
SDK_ROOT = $(FIRMWARE_ROOT)/dependencies/STM32CubeL4
HAL_ROOT = $(SDK_ROOT)/Drivers/STM32L4xx_HAL_Driver
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
SDK_ROOT = $(FIRMWARE_ROOT)/dependencies/STM32CubeL4
HAL_ROOT = $(SDK_ROOT)/Drivers/STM32L4xx_HAL_Driver
endif

ifeq ($(BOARD_VERSION), NUCLEO_L0)
SDK_ROOT = $(FIRMWARE_ROOT)/dependencies/STM32CubeL0
HAL_ROOT = $(SDK_ROOT)/Drivers/STM32L0xx_HAL_Driver
endif

LIB_ROOT = $(FIRMWARE_ROOT)/libraries
PROJ_ROOT = $(FIRMWARE_ROOT)/applications/SignBuddy

ifeq ($(BOARD_VERSION), DISCO_L4)
HAL_SOURCES = \
	$(HAL_ROOT)/Src/stm32l4xx_ll_adc.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_gpio.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_lpuart.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_rcc.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_tim.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_utils.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_usart.c 
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
HAL_SOURCES = \
	$(HAL_ROOT)/Src/stm32l4xx_ll_adc.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_gpio.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_lpuart.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_rcc.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_tim.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_utils.c \
	$(HAL_ROOT)/Src/stm32l4xx_ll_usart.c 
endif

ifeq ($(BOARD_VERSION), NUCLEO_L0)
HAL_SOURCES = \
	$(HAL_ROOT)/Src/stm32l0xx_ll_adc.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_gpio.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_lpuart.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_rcc.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_tim.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_utils.c \
	$(HAL_ROOT)/Src/stm32l0xx_ll_usart.c 
endif

LIB_SOURCES = \
	$(LIB_ROOT)/fast_fifo/fifo.c \
	$(LIB_ROOT)/system_time/system_time.c \

C_SOURCES = \
	main.c \
	adc.c \
	ble_uart.c \
	gpio.c \
	log_uart.c \
	logger.c \
	sensors.c \
	$(LIB_SOURCES) \
	$(HAL_SOURCES) \

ifeq ($(BOARD_VERSION), DISCO_L4)
C_SOURCES += system_stm32l4xx.c
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
C_SOURCES += system_stm32l4xx.c
endif

ifeq ($(BOARD_VERSION), NUCLEO_L0)
C_SOURCES += system_stm32l0xx.c
endif

C_INCLUDES = \
	-I$(SDK_ROOT)/Drivers/CMSIS/Include \
	-I$(HAL_ROOT)/Inc \
	-I$(PROJ_ROOT) \
	-I$(LIB_ROOT)/system_time \
	-I$(LIB_ROOT)/fast_fifo \

ifeq ($(BOARD_VERSION), DISCO_L4)
C_INCLUDES += -I$(SDK_ROOT)/Drivers/CMSIS/Device/ST/STM32L4xx/Include
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
C_INCLUDES += -I$(SDK_ROOT)/Drivers/CMSIS/Device/ST/STM32L4xx/Include
endif

ifeq ($(BOARD_VERSION), NUCLEO_L0)
C_INCLUDES += -I$(SDK_ROOT)/Drivers/CMSIS/Device/ST/STM32L0xx/Include
endif

ifeq ($(BOARD_VERSION), DISCO_L4)
STARTUP_FILE = startup_stm32l476xx.s
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
STARTUP_FILE = startup_stm32l476xx.s
endif

ifeq ($(BOARD_VERSION), NUCLEO_L0)
STARTUP_FILE = startup_stm32l053xx.s
endif

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
ifeq ($(BOARD_VERSION), DISCO_L4)
CPU = -mcpu=cortex-m4
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
CPU = -mcpu=cortex-m4
endif

ifeq ($(BOARD_VERSION), NUCLEO_L0)
CPU = -mcpu=cortex-m0plus
endif	

# fpu
ifeq ($(BOARD_VERSION), DISCO_L4)
FPU = -mfpu=fpv4-sp-d16
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
FPU = -mfpu=fpv4-sp-d16
endif

# float-abi
ifeq ($(BOARD_VERSION), DISCO_L4)
FLOAT-ABI = -mfloat-abi=hard
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
FLOAT-ABI = -mfloat-abi=hard
endif

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

ifeq ($(BOARD_VERSION), DISCO_L4)
CHIPSET_DEF = STM32L476xx
BOARD_DEF = BOARD_DISCO_L4
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
CHIPSET_DEF = STM32L476xx
BOARD_DEF = BOARD_NUCLEO_L4
endif

ifeq ($(BOARD_VERSION), NUCLEO_L0)
CHIPSET_DEF = STM32L053xx
BOARD_DEF = BOARD_NUCLEO_L0
endif

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS +=  \
	-D$(CHIPSET_DEF) \
	-D$(BOARD_DEF) \
	-DUSE_FULL_LL_DRIVER \


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
ifeq ($(BOARD_VERSION), DISCO_L4)
LDSCRIPT = STM32L476VGTx_FLASH.ld
endif

ifeq ($(BOARD_VERSION), NUCLEO_L4)
LDSCRIPT = STM32L476VGTx_FLASH.ld
endif

ifeq ($(BOARD_VERSION), NUCLEO_L0)
LDSCRIPT = STM32L053R8Tx_FLASH.ld
endif

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

flash: all
	# retry st-flash on failure
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000 || st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000
	st-flash reset

clean:
	-rm -fR $(BUILD_DIR)
  
-include $(wildcard $(BUILD_DIR)/*.d)

