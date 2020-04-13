# PINx,PORTx toggle test
include avr_setup.mk

help:
	@echo
	@echo "make OPT={yes|no} TEST={1|2|3} clean flash-{pro|uno}"
	@echo

clean:
	$(RM) toggle_pro.elf toggle_uno.elf *.o

flash-uno: toggle_uno.elf
	avrdude -p atmega328p -c arduino -P `sh detectusb.sh` -U flash:w:$^

flash-pro: toggle_pro.elf
	avrdude -p atmega32u4 -c avr109 -P `sh detectusb.sh` -U flash:w:$^

CFLAGS += -Wall -Wstrict-prototypes # -v
ifeq ($(strip $(OPT)),yes)
  CFLAGS += -O1
else
  CFLAGS += -O0
endif

ifeq ($(strip $(TEST)),1)
  CFLAGS += -DTEST_1
endif
ifeq ($(strip $(TEST)),2)
  CFLAGS += -DTEST_2
endif
ifeq ($(strip $(TEST)),3)
  CFLAGS += -DTEST_3
endif
