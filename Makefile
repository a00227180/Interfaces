# TODO: Use correct command for deleting files according to your OS

# This works for linux
RM := del /Q

TARGET := second_partial
STARTUP := startup

.PHONY:all
all: clean assemble compile link objcopy run

.PHONY:clean
clean:
	$(RM) *.o *.elf *.bin

assemble:
	arm-none-eabi-as -mcpu=arm926ej-s -g startup.s -o startup.o

compile:
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -g -c second_partial.c -o second_partial.o &&\
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -g -c uart.c -o uart.o

link:
	arm-none-eabi-ld -T second_partial.ld startup.o second_partial.o uart.o -o second_partial.elf

objcopy:
	arm-none-eabi-objcopy -O binary second_partial.elf second_partial.bin

run:
	qemu-system-arm -M versatilepb -cpu arm926 -kernel second_partial.bin -nographic

debug:
	qemu-system-arm -M versatilepb -m 128M -cpu arm926 -kernel hello.bin -nographic -s -S

disassemble:
	arm-none-eabi-objdump -D second_partial.elf > second_partial_disassembled.s
