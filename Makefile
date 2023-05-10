CROSS_COMPILE=riscv64-linux-gnu-
ATTR=-fno-stack-protector -nostartfiles
DD1K=dd of=build/fw.bin bs=1k conv=notrunc

ifeq ($(USE_256_FLASH),1)
	ATTR += -DUSE_256_FLASH
endif

default:
	mkdir -p build
	dtc -I dts -O dtb -o build/yuquan_sbi.dtb yuquan_sbi.dts
	dtc -I dts -O dtb -o build/yuquan_linux.dtb yuquan_linux.dts
	$(CROSS_COMPILE)gcc $(ATTR) -Os -c print.c -o build/print.o
	$(CROSS_COMPILE)gcc $(ATTR) -Os -c load_data.c -o build/load_data.o
	$(CROSS_COMPILE)gcc $(ATTR) -Os -c init.c -o build/init.o
	$(CROSS_COMPILE)gcc $(ATTR) -Os -c spi.c -o build/spi.o
	$(CROSS_COMPILE)gcc $(ATTR) -O3 -T./link.lds -Wl,--gc-sections \
		start.S build/init.o build/spi.o build/print.o build/load_data.o -o build/lowlevelboot.elf
	$(CROSS_COMPILE)objcopy -O binary -S build/lowlevelboot.elf build/lowlevelboot.bin
	$(CROSS_COMPILE)objdump --source --demangle --disassemble --reloc --wide build/lowlevelboot.elf > build/lowlevelboot.lst
	$(DD1K) count=0 if=/dev/zero
	$(DD1K) seek=0 if=build/lowlevelboot.bin
	$(DD1K) seek=2 if=build/yuquan_sbi.dtb
	$(DD1K) seek=4 if=build/yuquan_linux.dtb
	$(DD1K) seek=6 if=fw_jump.bin
	$(DD1K) seek=92 if=Image

clean:
	rm -rf build
