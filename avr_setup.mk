# Makefile default command macros switch to AVR
CC = avr-gcc
LD = avr-ld
AR = avr-ar
AS = avr-as
CXX = avr-g++
OBJCOPY = avr-objcopy
HEXFORMAT = ihex
HEXGEN = $(OBJCOPY) -O $(HEXFORMAT) -R .eeprom -R .fuse -R .lock -R .signature

%.elf: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

%.hex: %.elf
	$(HEXGEN) $^ $@

%.s: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -S $(OUTPUT_OPTION) $<

ifneq ($(LIST),)
  CFLAGS += -Wa,-adhlns=$*.lst,-gstabs,--listing-cont-lines=100
endif

ifneq ($(MAP),)
  LDFLAGS += -Wl,-Map=$*.map,--cref
endif

# select Arduino UNO, Pro Micro

# Arduino UNO configure
%_uno.o %_uno.s: CPPFLAGS += -DF_CPU=16000000L
%_uno.o %_uno.s: CFLAGS  += -mmcu=atmega328p
%_uno.elf:       LDFLAGS += -mmcu=atmega328p
%_uno.o: %_uno.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
%_uno.s: %_uno.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -S $(OUTPUT_OPTION) $<

# Pro Micro configure
%_pro.o %_pro.s: CPPFLAGS += -DF_CPU=16000000L
%_pro.o %_pro.s: CFLAGS  += -mmcu=atmega32u4
%_pro.elf:       LDFLAGS += -mmcu=atmega32u4
%_pro.o: %_pro.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
%_pro.s: %_pro.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -S $(OUTPUT_OPTION) $<
