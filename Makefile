#
#	Makefile for FreeRTOS demo on Raspberry Pi
#
BASE=$(shell pwd)/
BUILD_DIR=$(shell pwd)/build/

MODULE_NAME="RaspberryPi BSP"

TARGETS=kernel.img kernel.list kernel.syms kernel.elf
LINKER_SCRIPT=raspberrypi.ld

-include .dbuild/dbuild.mk

QEMU_PATH = /home/fser/code/qemu-raspi/arm-softmmu/
QEMU = $(QEMU_PATH)/qemu-system-arm



.PHONY: emu
emu: all
	$(QEMU) -kernel kernel.elf -cpu arm1176 -m 512 -M raspi -nographic

.PHONY: debug-emu
debug-emu: all
	$(QEMU) -s -S -kernel kernel.elf -cpu arm1176 -m 512 -M raspi -nographic


all: kernel.list kernel.img kernel.syms
	@$(SIZE) kernel.elf

kernel.img: kernel.elf
	$(Q)$(PRETTY) IMAGE $(MODULE_NAME) $@
	$(Q)$(OBJCOPY) kernel.elf -O binary $@

kernel.list: kernel.elf
	$(Q)$(PRETTY) LIST $(MODULE_NAME) $@
	$(Q)$(OBJDUMP) -D -S  kernel.elf > $@

kernel.syms: kernel.elf
	$(Q)$(PRETTY) SYMS $(MODULE_NAME) $@
	$(Q)$(OBJDUMP) -t kernel.elf > $@

#kernel.elf: LDFLAGS += -L "/opt/Xilinx/14.2/ISE_DS/EDK/gnu/arm/lin64/lib/gcc/arm-xilinx-eabi/4.6.1/" -lgcc
#kernel.elf: LDFLAGS += -L "/opt/Xilinx/14.2/ISE_DS/EDK/gnu/arm/lin64/arm-xilinx-eabi/lib/" -lc
kernel.elf: LDFLAGS += -L "/usr/lib/gcc/arm-none-eabi/4.9.3/" -lgcc
kernel.elf: LDFLAGS += -L "/usr/arm-none-eabi/lib/" -lc
kernel.elf: $(OBJECTS)
	$(Q)$(LD) $(OBJECTS) -Map kernel.map -o $@ -T $(LINKER_SCRIPT) $(LDFLAGS)
