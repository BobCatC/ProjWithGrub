.text
.global loader                   # making entry point visible to linker

# setting up the Multiboot header - see GRUB docs for details
.set FLAGS,    0x0               # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002        # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS)  # checksum required

.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM



# reserve initial kernel stack space
.set STACKSIZE, 0x10000           # that is, 16kb.
.lcomm stack, STACKSIZE          # reserve 16k stack
.comm  mbd, 4                    # we will use this in kmain
.comm  magic, 4                  # we will use this in kmain

loader:
movl  $(stack + STACKSIZE / 2), %esp # set up the stack
movl  %eax, magic                # Multiboot magic number
movl  %ebx, mbd                  # Multiboot data structure
movw $0x10, %ax
movw %ax, %ss
call  main                      # call C code
cli
hang:
hlt                              # halt machine should kernel return
jmp   hang
