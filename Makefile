# Hey Emacs, this is a -*- makefile -*-
#
# WinAVR makefile written by Eric B. Weddington, J�rg Wunsch, et al.
# Released to the Public Domain
# Please read the make user manual!

# mth 2004/09
# Differences from WinAVR 20040720 sample:
# - DEPFLAGS according to Eric Weddingtion's fix (avrfreaks/gcc-forum)
# - F_OSC Define in CFLAGS and AFLAGS

# MCU name
MCU = atmega328p

AVRDUDE_PORT = /dev/ttyACM99
USART_PORT = $(AVRDUDE_PORT)

# Main Oscillator Frequency
# This is only used to define F_OSC in all assembler and c-sources.
ifndef F_OSC
	F_OSC = 16000000
endif

ifdef WS2812_PAUSE_INTERRUPTS
	DEFAULT_FLAGS += -DWS2812_PAUSE_INTERRUPTS
endif

ifndef BAUD_INT
	BAUD_INT = 500000
endif
BAUD = $(BAUD_INT)UL

ifndef BUFFER_STATUS_BYTES
	BUFFER_STATUS_BYTES=0
endif
ifndef UART_INTERRUPT
	UART_INTERRUPT=1
endif

ifndef TIMERNR
	TIMERNR=1
endif

ifndef PRESCALER
    PRESCALER=64
endif

ifndef USE_MATH_LIB
	USE_MATH_LIB=0
	MATH_LIB = -lm
endif

# Export rewritten environment and new variables to each make fork.
export USE_MATH_LIB
export TIMERNR
export PRESCALER
export MAX_COUNTDOWNS=1
export MCU
export F_OSC
export BAUD_INT
export USART_PORT
export BUFFER_STATUS_BYTES
export UART_INTERRUPT

# make all = Make software.
#
# make clean = Clean out built project files.
#
# make coff = Convert ELF to AVR COFF (for use with AVR Studio 3.x or VMLAB).
#
# make extcoff = Convert ELF to AVR Extended COFF (for use with AVR Studio
#                4.07 or greater).
#
# make program = Download the hex file to the device, using avrdude.  Please
#                customize the avrdude settings below first!

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Target file name (without extension).
TARGET = main
TEST_TARGET = unittests

EXPLICIT_TARGETS = $(TARGET) $(TEST_TARGET)

# List C source files here. (C dependencies are automatically generated.)
COMMON_SRCS = ws2812.c color.c trans.c moods.c modes.c state.c fncs/*.c text/*.c utils/*.c
#SRC = $(TARGET).c $(COMMON_SRCS)

# Define all test object files.
TEST_SRCS = $(COMMON_SRCS) $(UNIT_SRC)
TEST_OBJ = $(TEST_SRCS:.c=.TEST.o)

# Definition of recursively build submodules
SUBMODULES := OdroidUart AVRClock
FIND_CMD = $(shell find $(module) -not -path '*test*' -name '*.o')
SUBOBJECTS = $(foreach module,$(SUBMODULES),$(FIND_CMD))

# A Whitespace, that is usable in e.g. $subst
SPACE:=
SPACE +=

# Automatically find all relevant source files by extension, ignore sources
# from sumodules and explicit targets.
IGNORE_PATTERN := $(SUBMODULES) $(EXPLICIT_TARGETS)
PATTERN := ${subst \ ,,.*(${subst ${SPACE},,${addprefix |,${IGNORE_PATTERN}}}).*}
FIXED_PATTERN := ${subst (|,(,$(PATTERN)}
AUTO_SOURCES := $(patsubst ./%,%,$(shell find -regextype egrep -not -regex '$(FIXED_PATTERN)' -iname "*.c"))
SOURCES := $(AUTO_SOURCES) $(TARGET).c

# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC =

# Optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#DEBUG = stabs
DEBUG = dwarf-2

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS =

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options here
CDEFS =

# Place -I options here
CINCS =

# Compiler flags.
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
DEFAULT_FLAGS += -g$(DEBUG)
DEFAULT_FLAGS += $(CDEFS) $(CINCS)
DEFAULT_FLAGS += -O$(OPT)
DEFAULT_FLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
DEFAULT_FLAGS += -Wall -Wstrict-prototypes
DEFAULT_FLAGS += -Wa,-adhlns=$(<:.c=.lst)
DEFAULT_FLAGS += -Werror
DEFAULT_FLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
DEFAULT_FLAGS += $(CSTANDARD)

CFLAGS = $(DEFAULT_FLAGS)
CFLAGS += -DF_OSC=$(F_OSC)
CFLAGS += -DF_CPU=$(F_OSC)
CFLAGS += -DBAUD=$(BAUD)

# Assembler flags.
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
ASFLAGS += -DF_OSC=$(F_OSC)
ASFLAGS += -DF_CPU=$(F_OSC)

#Additional libraries.

# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min
# Floating point printf version (requires MATH_LIB = -lm below)

PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt
PRINTF_LIB =

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt
SCANF_LIB =

# External memory options

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--defsym=__heap_start=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

# Programming support using avrdude. Settings and variables.

# Programming hardware: alf avr910 avrisp bascom bsd
# dt006 pavr picoweb pony-stk200 sp12 stk200 stk500
#
# Type: avrdude -c ?
# to get a full listing.
#
AVRDUDE_PROGRAMMER = arduino

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep
AVRDUDE_READ_EEPROM = -U eeprom:r:eeprom.hex:i

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE_COUNTER = -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_NO_VERIFY = -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude>
# to submit bug reports.
AVRDUDE_VERBOSE = -v -v

AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)

# Define programs and commands.
SHELL = sh
CC = avr-gcc
GCC = gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
COPY = cp

# Define all object files.
OBJ = $(SOURCES:.c=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(SOURCES:.c=.lst)

# Compiler flags to generate dependency files.
### GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

CFLAGS += -DTIMERNR=$(TIMERNR)
CFLAGS += -DPRESCALER=$(PRESCALER)
CFLAGS += -ftrack-macro-expansion=0
CFLAGS += -fno-diagnostics-show-caret
CFLAGS += -fdiagnostics-color=auto

# Configuration backups
UART_CFG = .usart.ini
CONT_CFG = control.meta

_CFG_FILES = $(UART_CFG) $(CONT_CFG)

CFG_SRC_DIR = OdroidUart/
CFG_BAK_DIR = .cfgs/

CFGS = $(addprefix $(CFG_SRC_DIR),$(_CFG_FILES))
CFG_BACKUPS = $(subst $(CFG_SRC_DIR),$(CFG_BAK_DIR),$(CFGS))

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

TEST_FLAGS = -lm
TEST_FLAGS += -lcunit

TARGET_TESTS = $(TESTED_SRCS:.c=.unittest)
TESTED_SRCS = trans.c color.c
UNIT_SRC = unittests.c

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_BACKUP = Backup config
MSG_CLEAN_TESTS = Cleaning up unittests

# Default target.
all: begin gccversion sizebefore build sizeafter finished end

test : begin runtest cleantests end

readeeprom : begin geteeprom finished end

geteeprom :
	@echo
	@echo $(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_READ_EEPROM)
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_READ_EEPROM)

cleantests :
	@echo
	$(REMOVE) $(TARGET_TESTS)

runtest : $(TARGET_TESTS)

%.unittest: %.c unittests.c unittest.h
	@echo
	@echo comiling $@
	$(GCC) -include $(subst .c,.h,$<) -DUNITTEST $(TEST_FLAGS) $(CFLAGS) $(UNIT_SRC) $< -o $@
	./$@

%.TEST.o: %.c
	@echo
	$(GCC) -DUNITTEST -c $(CFLAGS) $< -o $@

build: backupconfig subsystems elf hex eep lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym

subsystems:
	for dir in $(SUBMODULES) ; do \
        $(MAKE) -C  $$dir ; \
    done

clean_subsystems:
	for dir in $(SUBMODULES) ; do \
        $(MAKE) -C  $$dir clean ; \
    done

# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi

# Display compiler version information.
gccversion :
	@$(CC) --version

# Program the device.
program: subsystems $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)
	$(SIZE) -C --mcu=$(MCU) $(TARGET).elf

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000

backupconfig : ${CFG_BACKUPS}

${CFG_BACKUPS} :
	@echo
	@echo $(MSG_BACKUP) $@
	@mkdir -p $(@D)
	@touch $(subst $(CFG_BAK_DIR),$(CFG_SRC_DIR),$@) $@
	@cp -f $(subst $(CFG_BAK_DIR),$(CFG_SRC_DIR),$@) $@

coff: $(TARGET).elf
	@echo
	@echo $(MSG_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof


extcoff: $(TARGET).elf
	@echo
	@echo $(MSG_EXTENDED_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $(OBJ) $(SUBOBJECTS) --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Target: clean project.
clean: begin clean_list clean_subsystems finished end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(SOURCES:.c=.s)
	$(REMOVE) $(SOURCES:.c=.d)
	$(REMOVE) .dep/*

listsources :
	@echo
#	@echo $(FIND) $(FIND_FLAGS)
#	@find $(FIND_FLAGS)
	@echo $(AUTO_SOURCES)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff cleantests \
clean clean_list program subsystems clean_subsystems ${CFG_BACKUPS} test