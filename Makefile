CURR_DIR = $(shell basename $(CURDIR))
BUILD_DIR = build
PROJECT=$(CURR_DIR)

CCPREFIX = arm-none-eabi-
LD=$(CCPREFIX)cc
CC=$(CCPREFIX)gcc
AR=$(CCPREFIX)ar
AS=$(CCPREFIX)gcc -x assembler-with-cpp
#AS=$(CCPREFIX)as
CP=$(CCPREFIX)objcopy
OD=$(CCPREFIX)objdump
SE=$(CCPREFIX)size
GDB=$(CCPREFIX)gdb

# List all C defines here
DDEFS = -DSTM32F40XX -DUSE_STDPERIPH_DRIVER

SF=st-flash
MCU=cortex-m4

ASRC = startup/startup_stm32.s
SRC  = ./src/main.c
SRC += ./src/system_stm32f4xx.c
SRC += ./src/syscalls.c
SRCDIRS := $(shell find . -name '*.c' -exec dirname {} \; | uniq)
ASMDIRS := $(shell find . -name '*.s' -exec dirname {} \; | uniq)

STMSPDDIR    = STM32F4xx_StdPeriph_Driver
STMSPSRCDDIR = $(STMSPDDIR)/src
STMSPINCDDIR = $(STMSPDDIR)/inc

## used parts of the STM-Library
#SRC += $(STMSPSRCDDIR)/stm32f4xx_adc.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_cec.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_crc.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_comp.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_dac.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_dbgmcu.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_dma.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_exti.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_flash.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_gpio.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_syscfg.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_i2c.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_iwdg.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_pwr.c
SRC += $(STMSPSRCDDIR)/stm32f4xx_rcc.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_rtc.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_spi.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_tim.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_usart.c
#SRC += $(STMSPSRCDDIR)/stm32f4xx_wwdg.c
#SRC += $(STMSPSRCDDIR)/misc.c

STARTUP = startup/startup_stm32.s

# List all include directories here
INCDIRS = ./inc ./inc/CMSIS $(STMSPINCDDIR)
              
# List the user directory to look for the libraries here
LIBDIRS += ./inc/user_libs
#LIBDIRS += -I./inc/usb_lib
#LIBDIRS += -I./inc/tm_lib
 
# List all user libraries here
LIBS = ./inc/user_libs/debug.c 
LIBS += ./inc/user_libs/SysTickDelay.c

# Define optimisation level here
# -O0 (do no optimization, the default if no optimization level is specified)
# -O1 (optimize minimally)
# -O2 (optimize more)
# -O3 (optimize even more)
# -Ofast (optimize very aggressively to the point of breaking standard compliance)
# -Og (Optimize debugging experience - enables optimizations that do not interfere with debugging  
# -Os (Optimize for size - enables all -O2 optimizations that do not typically increase code size
OPT = -O0
 
# Define linker script file here
LINKER_SCRIPT = ./LinkerScript.ld

INCDIR  = $(patsubst %,-I%, $(INCDIRS))
LIBDIR  = $(patsubst %,-I%, $(LIBDIRS))
LIB     = $(patsubst %,-l%, $(LIBS))

# run from Flash
DEFS    = $(DDEFS) -DRUN_FROM_FLASH=1

OBJS  = $(STARTUP:.s=.o) $(SRC:.c=.o) $(LIBS:.c=.o)

MCFLAGS = -mcpu=$(MCU)
 
ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb   
CPFLAGS = $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb   -fomit-frame-pointer -Wall -Wstrict-prototypes -fverbose-asm  $(DEFS)
LDFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb  -lrdimon -T$(LINKER_SCRIPT) -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref,--no-warn-mismatch  
#LDFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb -lrdimon -nostartfiles -T$(LINKER_SCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LIBDIR) 

#Formatas
COLOR_BEGIN=\033[1;33m
COLOR_END=\033[0m

DEB_COL_BEG=\033[1;31m 
DEB_COL_END=\033[0m 

.PHONY: all clean debug prep_release prep_debug release flash_swd flash_uart

ELF_FILE=$(PROJECT).elf 
BIN_FILE=$(PROJECT).bin
#
# Debug build settings
#
DBG_CPFLAGS = -Wa,-ahlms=$(BUILD_DIR)/debug/$(<:.c=.lst)
DBG_ASFLAGS = -Wa,-amhls=$(BUILD_DIR)/debug/$(<:.s=.lst)
DBGDIR = $(BUILD_DIR)/debug
DBGELF = $(DBGDIR)/$(ELF_FILE)
DBGBIN = $(DBGDIR)/$(BIN_FILE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBG_LDFLAGS = -lc

#
# Release build settings
#
REL_CPFLAGS = -Wa,-ahlms=$(BUILD_DIR)/release/$(<:.c=.lst)
REL_ASFLAGS = -Wa,-amhls=$(BUILD_DIR)/release/$(<:.s=.lst)
RELDIR = $(BUILD_DIR)/release
RELELF = $(RELDIR)/$(ELF_FILE)
RELBIN = $(RELDIR)/$(BIN_FILE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))

# Default build
all: release

#
# Debug rules
#
debug: prep_debug $(DBGBIN)
	@echo "$(DEB_COL_BEG)[DEBUG]$(DEB_COL_END)"
	$(TRGT)size $(DBGELF)

$(DBGBIN): $(DBGELF)
	@echo "$(COLOR_BEGIN) >>>  Generating raw binary file... $(COLOR_END)"
	$(CP) -O binary -S  $< $@

$(DBGELF): $(DBGOBJS)
	@echo "$(COLOR_BEGIN) >>>  Linking $< into $@ file... $(COLOR_END)"
	$(CC) $(DBGOBJS) $(LDFLAGS) $(DBG_LDFLAGS) -o $@

$(DBGDIR)/%.o: %.c
	@echo "$(COLOR_BEGIN) >>>  Compiling $< into $@ file$(COLOR_END)"
	$(CC) -c $(CPFLAGS) $(DBG_CPFLAGS) -I . $(INCDIR) -I$(LIBDIRS) $< -o $@

$(DBGDIR)/%.o: %.s 
	@echo "$(COLOR_BEGIN) >>>  Compiling $< into $@ file ... $(COLOR_END)"
	$(AS) -c $(ASFLAGS) $(DBG_ASFLAGS) $< -o $@
	@echo "$(DEB_COL_BEG) >>>  Printing variables... $(DEB_COL_END)"

#
# Release rules
#
release: prep_release $(RELBIN)
	@echo "$(COLOR_BEGIN)[RELEASE]$(COLOR_END)"
	$(TRGT)size $(RELELF)
 
$(RELBIN): $(RELELF)
	@echo "$(COLOR_BEGIN) >>>  Generating raw binary file... $(COLOR_END)"
	$(CP) -O binary -S  $< $@

$(RELELF): $(RELOBJS)
	@echo "$(COLOR_BEGIN) >>>  Linking into $@ file... $(COLOR_END)"
	$(CC) $(RELOBJS) $(LDFLAGS) -o $@

$(RELDIR)/%.o: %.c 
	@echo "$(COLOR_BEGIN) >>>  Compiling source $< into $@$(COLOR_END)"
	$(CC) -c $(CPFLAGS) $(REL_CPFLAGS) -I . $(INCDIR) -I$(LIBDIRS) $< -o $@
	
$(RELDIR)/%.o: %.s 
	@echo "$(COLOR_BEGIN) >>>  Compiling ASM file $< into $@$(COLOR_END)"
	$(AS) -c $(ASFLAGS) $(REL_ASFLAGS) $< -o $@
	@echo "$(DEB_COL_BEG) >>>  Printing variables... $(DEB_COL_END)"

#
# Flashing rules
#
flash_swd: release 
	@echo "$(COLOR_BEGIN) >>>  Flashing... $(COLOR_END)"
	st-flash write $(RELBIN) 0x8000000

flash_debug: debug 
	@echo "$(DEB_COL_BEG) >>>  Flashing... $(DEB_COL_END)"
	st-flash write $(DBGBIN) 0x8000000
	@echo "$(DEB_COL_BEG) >>>  Starting openocd... $(DEB_COL_END)"
	xterm -e 'openocd -f interface/stlink-v2.cfg -f target/stm32f4x_stlink.cfg' &
	sleep 1 
	@echo "$(DEB_COL_BEG) >>>  Starting gdb... $(DEB_COL_END)"
	$(GDB) $(DBGELF)

flash_uart: release
	@echo "$(COLOR_BEGIN) >>>  Flashing ... $(COLOR_END)"
	st-flash write $(RELBIN) 0x8000000

erase:
	@echo "$(COLOR_BEGIN) >>>  Erasing flash memory ... $(COLOR_END)"
	st-flash erase

start_gdb: 
	@echo "$(DEB_COL_BEG) >>>  Starting openocd... $(DEB_COL_END)"
	xterm -e 'openocd -f interface/stlink-v2.cfg -f target/stm32f4x_stlink.cfg' &
	sleep 1
	@echo "$(DEB_COL_BEG) >>>  Starting gdb... $(DEB_COL_END)"
	$(GDB) $(DBGELF)


#
# Other rules
#
prep_release:
	mkdir -p $(SRCDIRS:./%=build/release/%)
	mkdir -p $(ASMDIRS:./%=build/release/%)
		
prep_debug:
	mkdir -p $(SRCDIRS:./%=build/debug/%)
	mkdir -p $(ASMDIRS:./%=build/debug/%)

clean:
	rm -r $(BUILD_DIR)
