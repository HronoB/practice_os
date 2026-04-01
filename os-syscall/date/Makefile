OBJS = bio.o console.o exec.o file.o fs.o ide.o ioapic.o kalloc.o kbd.o lapic.o \
       log.o main.o mp.o picirq.o pipe.o proc.o sleeplock.o spinlock.o string.o \
       swtch.o syscall.o sysfile.o sysproc.o trapasm.o trap.o uart.o vectors.o vm.o

TOOLPREFIX = i386-jos-elf-
QEMU = qemu-system-i386
CC = $(TOOLPREFIX)gcc
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump

CFLAGS = -fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -m32 -Werror \
         -fno-omit-frame-pointer -fno-stack-protector -nostdinc -I.
ASFLAGS = -m32 -gdwarf-2 -Wa,-divide
LDFLAGS = -m elf_i386 -nostdlib -N -z max-page-size=0x1000

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.S
	$(CC) $(ASFLAGS) -c $<

xv6.img: bootblock kernel fs.img
	dd if=/dev/zero of=xv6.img count=10000
	dd if=bootblock of=xv6.img conv=notrunc
	dd if=kernel of=xv6.img seek=1 conv=notrunc

# FIXED BOOTBLOCK - точно как оригинал
bootblock: bootasm.S bootmain.c
	$(CC) $(CFLAGS) -fno-pic -O -nostdinc -I. -c bootmain.c
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I. -c bootasm.S
	$(LD) -N -e start -Ttext 0x7C00 bootasm.o bootmain.o -o bootblock.o
	$(OBJDUMP) -S bootblock.o > bootblock.asm
	$(OBJCOPY) -S -O binary -j .text bootblock.o bootblock
	perl ./sign.pl bootblock
	@hexdump -C bootblock | tail -1 | grep "55 aa" && echo "✓ Bootblock SIGNED ✓" || (echo "✗ Bootblock SIGN FAILED"; false)

kernel: $(OBJS) entry.o entryother initcode kernel.ld
	$(LD) -T kernel.ld entry.o $(OBJS) -b binary initcode entryother -o kernel
	$(OBJDUMP) -S kernel > kernel.asm

entryother: entryother.S
	$(CC) $(ASFLAGS) -nostdinc -I. -c entryother.S
	$(LD) -N -e start -Ttext 0x7000 entryother.o -o entryother.elf
	$(OBJCOPY) -S -O binary -j .text entryother.elf entryother

initcode: initcode.S
	$(CC) $(ASFLAGS) -nostdinc -I. -c initcode.S
	$(LD) -N -e start -Ttext 0 initcode.o -o initcode.elf
	$(OBJCOPY) -S -O binary -j .text initcode.elf initcode

vectors.S: vectors.pl
	perl vectors.pl > vectors.S

# FIXED USER PROGRAMS - без дублирования printf.o
UPROGS=_cat _echo _forktest _grep _init _kill _ln _ls _mkdir _rm _sh _stressfs _usertests _wc _zombie _date

# ULIB (один раз!)
ulib.o: ulib.c
	$(CC) $(CFLAGS) -c ulib.c
usys.o: usys.S
	$(CC) $(ASFLAGS) -c usys.S
printf.o: printf.c
	$(CC) $(CFLAGS) -c printf.c
umalloc.o: umalloc.c
	$(CC) $(CFLAGS) -c umalloc.c

ULIB=ulib.o usys.o printf.o umalloc.o

# User programs - ЯВНЫЕ правила БЕЗ printf.o дублирования
_cat: cat.c $(ULIB)
	$(CC) $(CFLAGS) -c cat.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 cat.o $(ULIB) -o _cat

_echo: echo.c $(ULIB)
	$(CC) $(CFLAGS) -c echo.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 echo.o $(ULIB) -o _echo

_forktest: forktest.c ulib.o usys.o
	$(CC) $(CFLAGS) -c forktest.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 forktest.o ulib.o usys.o -o _forktest

_grep: grep.c $(ULIB)
	$(CC) $(CFLAGS) -c grep.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 grep.o $(ULIB) -o _grep

_init: init.c $(ULIB)
	$(CC) $(CFLAGS) -c init.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 init.o $(ULIB) -o _init

_kill: kill.c $(ULIB)
	$(CC) $(CFLAGS) -c kill.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 kill.o $(ULIB) -o _kill

_ln: ln.c $(ULIB)
	$(CC) $(CFLAGS) -c ln.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 ln.o $(ULIB) -o _ln

_ls: ls.c $(ULIB)
	$(CC) $(CFLAGS) -c ls.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 ls.o $(ULIB) -o _ls

_mkdir: mkdir.c $(ULIB)
	$(CC) $(CFLAGS) -c mkdir.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 mkdir.o $(ULIB) -o _mkdir

_rm: rm.c $(ULIB)
	$(CC) $(CFLAGS) -c rm.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 rm.o $(ULIB) -o _rm

_sh: sh.c $(ULIB)
	$(CC) $(CFLAGS) -c sh.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 sh.o $(ULIB) -o _sh

_stressfs: stressfs.c $(ULIB)
	$(CC) $(CFLAGS) -c stressfs.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 stressfs.o $(ULIB) -o _stressfs

_usertests: usertests.c $(ULIB)
	$(CC) $(CFLAGS) -c usertests.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 usertests.o $(ULIB) -o _usertests

_wc: wc.c $(ULIB)
	$(CC) $(CFLAGS) -c wc.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 wc.o $(ULIB) -o _wc

_zombie: zombie.c $(ULIB)
	$(CC) $(CFLAGS) -c zombie.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 zombie.o $(ULIB) -o _zombie

_date: date.c $(ULIB)
	$(CC) $(CFLAGS) -c date.c
	$(LD) $(LDFLAGS) -e main -Ttext 0 date.o $(ULIB) -o _date

fs.img: mkfs README $(UPROGS)
	./mkfs fs.img README $(UPROGS)

mkfs: mkfs.c fs.h
	gcc -Werror -Wall -o mkfs mkfs.c

# CLion
.PHONY: clion-qemu-nox clion-build clion-clean

clion-build:
	@echo "=== Building xv6 for Mac ==="
	make clean
	make vectors.S bootblock kernel $(UPROGS) fs.img xv6.img
	@echo "✓ Build complete - LAUNCHING QEMU"

clion-qemu-nox: clion-build
	@echo "*** Ctrl-a x to exit QEMU ***"
	$(QEMU) -nographic -drive file=fs.img,if=ide,index=1,media=disk,format=raw \
		-drive file=xv6.img,if=ide,index=0,media=disk,format=raw -smp 2 -m 512

clion-clean:
	rm -f *.o *.asm *.elf bootblock entryother initcode kernel xv6.img fs.img mkfs $(UPROGS) vectors.S

clean: clion-clean