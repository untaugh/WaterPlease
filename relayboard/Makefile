PRG            = relayboard
OBJ            = $(PRG).o relay.o relaytimer.o spislave.o water.o cmd.o 
MCU_TARGET     = attiny45

DEFS           =
LIBS           = 
CC             = avr-gcc

override CFLAGS        = -g -Wall -Wextra -O2 -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst hex

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	avr-size -C --mcu=$(MCU_TARGET) $(PRG).elf
main.o: main.c 

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

text: hex bin srec

hex:  $(PRG).hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@


program:
	avrdude -v -p$(MCU_TARGET) -cusbtiny -Uflash:w:$(PRG).hex:i
