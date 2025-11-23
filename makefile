# Makefile for SSD1306 SPI project

CC = sdcc
MCU = -mpic16 -p18f4620
CFLAGS = $(MCU) --use-non-free -I.

SOURCES = spi_display.c
TARGET = spi_display

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).c
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET).hex

clean:
	rm -f *.hex *.cod *.cof *.lst *.map *.asm

program:
	@echo "Load $(TARGET).hex into PicSimLab"
