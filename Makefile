CC      = g++
CFLAGS  = -Wall -fno-builtin -nostdinc -nostdlib -ggdb3
LD      = ld

OBJFILES = \
	loader.o  \
	bin/printf.o  \
	bin/screen.o  \
	bin/registers.o \
	bin/cache_controll.o \
	bin/port_io.o \
	bin/other.o \
	bin/kernel.o  \
	bin/render.o \
	bin/memory.o \
	bin/interface.o \

image:
	@echo "Creating hdd.img..."
	@dd if=/dev/zero of=./hdd.img bs=512 count=16065 2>&1

	@echo "Creating bootable first FAT32 partition..."
	@sudo losetup /dev/loop1 ./hdd.img
	@(echo c; echo u; echo n; echo p; echo 1; echo ;  echo ; echo a; echo 1; echo t; echo c; echo w;) | sudo fdisk /dev/loop1 2>&1 || true

	@echo "Mounting partition to /dev/loop2..."
	@sudo losetup /dev/loop2 ./hdd.img \
    --offset    `echo \`sudo fdisk -lu /dev/loop1 | sed -n 10p | awk '{print $$3}'\`*512 | bc` \
    --sizelimit `echo \`sudo fdisk -lu /dev/loop1 | sed -n 10p | awk '{print $$4}'\`*512 | bc` 2>&1
	@sudo losetup -d /dev/loop1

	@echo "Format partition..."
	@sudo mkdosfs /dev/loop2

	@echo "Copy kernel and grub files on partition..."
	@sudo mkdir -p tempdir
	@sudo mount /dev/loop2 tempdir
	@sudo mkdir tempdir/boot
	@sudo cp -r grub tempdir/boot/
	@sudo cp bin/kernel.bin tempdir/
	@sleep 1
	@sudo umount /dev/loop2
	@sudo rm -r tempdir
	@sudo losetup -d /dev/loop2

	@echo "Installing GRUB..."
	@echo "device (hd0) hdd.img \n \
	       root (hd0,0)         \n \
	       setup (hd0)          \n \
	       quit\n" | grub --batch 1>/dev/null
	@echo "Done!"

all:
	make kernel.bin
	make image
	make qemu
	make copy_hdd

qemu:
	qemu-system-i386 -hda ./hdd.img
qemu_dbg:
	qemu-system-i386 -s -S -hda ./hdd.img &
	cgdb bin/kernel.bin.dbg

rebuild: clean all
.s.o:
	g++ -o $@ $<
.c.o:
	$(CC) -Iinclude $(CFLAGS) -o $@ -c $<

kernel.bin: 
	make compile

clean:
	rm -f $(OBJFILES) hdd.img kernel.bin

compile:
	echo -----------------------------------
	echo Compiling
	g++ -O0 -Iinclude -fno-builtin -nostdinc -nostdlib -o bin/kernel.o -c common/kernel.cpp
	gcc -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o loader.o -c common/loader.s
	gcc -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/cache_controll.o  -c common/assembler/cache_controll.s
	gcc -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/other.o  -c common/assembler/other.s
	gcc -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/port_io.o  -c common/assembler/port_io.s	
	gcc -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/registers.o  -c common/assembler/registers.s
	g++ -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/interface.o -c common/interface.cpp
	g++ -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/cache.o -c common/cache.cpp
	g++ -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/types.o -c common/types.cpp
	g++ -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/memory.o -c common/memory.cpp
	g++ -O0 -std=c99 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/printf.o -c common/printf.c
	g++ -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/program.o -c common/program.cpp
	g++ -O0 -Iinclude -fno-builtin -nostdinc -nostdlib -o bin/screen.o -c common/screen.cpp
	g++ -O0 -Iinclude -Wall -fno-builtin -nostdinc -nostdlib -o bin/console_view.o -c common/console_view.cpp


	ld -T linker.ld -o bin/kernel.bin bin/kernel.o loader.o bin/cache_controll.o bin/other.o bin/port_io.o bin/registers.o bin/interface.o bin/cache.o bin/types.o bin/memory.o bin/printf.o bin/program.o bin/screen.o bin/console_view.o

umount:
	sudo umount /dev/loop2
again:
	sudo losetup -d /dev/loop1
	sudo losetup -d /dev/loop2

copy_grub32:
	cp /usr/lib/grub/i386-pc/stage1 ./grub/	
	cp /usr/lib/grub/i386-pc/stage2 ./grub/
	cp /usr/lib/grub/i386-pc/fat_stage1_5 ./grub/
copy_grub64:
	cp /usr/lib/grub/x86_64-pc/stage1 ./grub/	
	cp /usr/lib/grub/x86_64-pc/stage2 ./grub/
	cp /usr/lib/grub/x86_64-pc/fat_stage1_5 ./grub/

copy_hdd:
	sudo rm -f /home/a/hdd.img
	cp ./hdd.img /home/a/
	
