CC = arm-none-eabi
COPS = -Wall -O2 -nostartfiles -ffreestanding -march=armv6zk -mtune=arm1176jzf-s
DEPS =  vectors.s characters.c console.c debug.c drawing.c framebuffer.c gpio.c helper.c interrupts.c mailbox.c main.c memory.c systemTimer.c

all: kernel.img

kernel.img:
	$(CC)-gcc $(COPS) $(DEPS) -o build/kernel.elf
	$(CC)-objdump -D build/kernel.elf > build/kernel.list
	$(CC)-objcopy build/kernel.elf -O binary kernel.img
	
clean:
	rm -f build/*.o
	rm -f *.img
	rm -f build/*.list
	rm -f build/*.map
	rm -f build/*.elf