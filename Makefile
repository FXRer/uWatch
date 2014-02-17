
###############################################################################
# Makefile for uWatch
###############################################################################


## General Flags
PROJECT = uWatch
TARGET = build/$(PROJECT).elf
CC = msp430-gcc

MSPDEBUG = mspdebug
PROGFLAGS = rf2500

## MCU to pass to the linker
MCU = msp430fr5739

## C flags
#CFLAGS := -Wall -Os -mmcu=$(MCU) -nostartfiles #-gdwarf-2 

CFLAGS := -Wall -Os -mmcu=$(MCU)

## Assembly flags
ASMFLAGS := -x assembler-with-cpp

## Linker flags
LDFLAGS = -mmcu=$(MCU) -Wl,--relax


SRC_DIR   := cocoOS drivers tasks
BUILD_DIR := build 

## Do not edit below this line.


SRC_A       := $(wildcard src/*.c)
SRC_A 	  += $(wildcard src/cocoOS/*.c)
SRC_A 	  += $(wildcard src/drivers/*.c)
SRC_A 	  += $(wildcard src/tasks/*.c)

SRC       := $(patsubst src/%.c,%.c,$(SRC_A))


#ASM       := $(foreach adir,$(SRC_DIR),$(wildcard src/$(adir)/*.s))

#ASM_OBJ	  := $(patsubst src/%.s,build/%.o,$(ASM))

#OBJ       = $(patsubst src/%.c,build/%.o,$(SRC))
OBJ_A 	:= $(SRC:.c=.o)
OBJ     = $(OBJ_A:%.o=build/%.o)

DEP	  := $(OBJ:%.o=%.d)
INCLUDES  := $(addprefix -I,$(SRC_DIR))
INCLUDES += -I.



all: checkdirs $(OBJ) $(TARGET) size

#-include $(DEP)

checkdirs: 
	@mkdir -p build
	@mkdir -p build/tasks
	@mkdir -p build/cocoOS
	@mkdir -p build/drivers


clean:
	@rm -rf build/



# Build ASM src files
#%.o : $(SRC_DIR)/%.s
#	@echo "ASM" $< " > " $@
#	@$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@
#	@msp430-size $@

# Build dir must be built in order to compile
#$(ASM_OBJ): | $(BUILD_DIR)

ec:
	@echo $(OBJ)

# Build C src files
build/%.o: src/%.c
#"CC " $< " > " $@
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -MD -o $@
	@msp430-size $(patsubst src/%,build/%,$@)



	
	
# Compile *.elf target from objects	
$(TARGET): $(OBJ) $(ASM_OBJ)
	@echo "=========================="
	@echo "$^ >" $@
	@$(CC) $(LDFLAGS) $(LIBDIRS) $(LIBS) $^ -o $@
	@echo "=========================="

size: $(TARGET)
#	@echo
	@msp430-size ${TARGET}

prog: $(TARGET)
	$(MSPDEBUG) $(PROGFLAGS) 'prog $<'


