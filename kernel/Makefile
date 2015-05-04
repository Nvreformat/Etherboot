RASPPI ?= 1
PREFIX ?= arm-none-eabi-
MAP = kernel.map

CC = $(PREFIX)gcc
AS = $(CC)
LD = $(PREFIX)ld
AR = $(PREFIX)ar

ifeq ($(strip $(RASPPI)),1)
ARCH ?= -march=armv6j -mtune=arm1176jzf-s -mfloat-abi=hard 
else
ARCH ?= -march=armv7-a -mtune=cortex-a7 -mfloat-abi=hard
endif
AFLAGS += $(ARCH) -g -I include -DRASPPI=$(RASPPI)
CFLAGS += $(ARCH) -g -I include -Wall -Wno-psabi -fsigned-char -std=gnu99 -include default.h -DRASPPI=$(RASPPI) -O

LIBS = libuspi.a libgcc.a

SOURCE = source/
BUILD = build/

OBJS += $(patsubst $(SOURCE)%.S, $(BUILD)%.o, $(wildcard $(SOURCE)*.S)) $(patsubst $(SOURCE)%.c, $(BUILD)%.o, $(wildcard $(SOURCE)*.c))
OBJS += $(patsubst $(SOURCE)/Drivers/%.S,$(BUILD)%.o,$(wildcard $(SOURCE)/Drivers/*.S)) $(patsubst $(SOURCE)/Drivers/%.c,$(BUILD)%.o,$(wildcard $(SOURCE)/Drivers/*.c))
OBJS += $(patsubst $(SOURCE)/Misc/%.S,$(BUILD)%.o,$(wildcard $(SOURCE)/Misc/*.S)) $(patsubst $(SOURCE)/Misc/%.c,$(BUILD)%.o,$(wildcard $(SOURCE)/Misc/*.c))
OBJS += $(patsubst $(SOURCE)/Lib/%.S,$(BUILD)%.o,$(wildcard $(SOURCE)/Lib/*.S)) $(patsubst $(SOURCE)/Lib/%.c,$(BUILD)%.o,$(wildcard $(SOURCE)/Lib/*.c))

$(BUILD)%.o: $(SOURCE)%.S $(BUILD)
	$(AS) $(AFLAGS) -c -o $@ $<

$(BUILD)%.o: $(SOURCE)%.c $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(BUILD)%.o: $(SOURCE)Drivers/%.S $(BUILD)
	$(AS) $(AFLAGS) -c -o $@ $<

$(BUILD)%.o: $(SOURCE)Drivers/%.c $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(BUILD)%.o: $(SOURCE)Misc/%.S $(BUILD)
	$(AS) $(AFLAGS) -c -o $@ $<

$(BUILD)%.o: $(SOURCE)Misc/%.c $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(BUILD)%.o: $(SOURCE)Lib/%.S $(BUILD)
	$(AS) $(AFLAGS) -c -o $@ $<

$(BUILD)%.o: $(SOURCE)Lib/%.c $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

all: kernel.img

kernel.img: $(OBJS) $(LIBS)
	$(LD) --no-undefined -L. -Map $(MAP) -o kernel.elf -Map kernel.map -T kernel.ld $(OBJS) $(LIBS)
	$(PREFIX)objdump -D kernel.elf > kernel.lst
	$(PREFIX)objcopy kernel.elf -O binary ../kernel.img

clean:
	rm -rf *.o *.elf *.lst *.img *.cir *.map *~ $(EXTRACLEAN)
