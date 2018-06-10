.text

.global _check_mem
.global _check_stack
.global _check_stack2
.global _Sleep
.global _do_rdtsc
.global _do_and_save_rdtsc
.global _get_cr0
.global _get_ds
.global _get_cs
.global _get_ss
.global _get_2_tsc
.global _flush_cache
.global _set_real_mode

.global _in_b
.global _out_b

.global _start_up
.global _add_new_address
.global _get_time_access
.global _get_time_access_novar
.global _end_of_work

.comm 	__ret_address__, 4
.comm 	__first_par__,  4
.comm   __second_par__, 4



_check_stack:
    popl %eax
    pushl %eax
    ret
# void _check_stack2(u32)
_check_stack2:
    popl %eax
    popl %ebx
    pushl %eax
    pushl %eax
    ret

_do_rdtsc:
    rdtsc
    ret


tsc_low:
.long 0x0
tsc_high:
.long 0x0

_do_and_save_rdtsc:
    pushl %edx
    rdtsc

    movl %eax, tsc_high

    movl %edx, tsc_low
    popl %edx
    ret


#|---------------------------------------|#
#|             void _Sleep(u32 nMS)      |#
#|---------------------------------------|#

MS_1:
.short 1000

_Sleep:

    popl __ret_address__
    popl __first_par__
    pushl __ret_address__

    pushw %dx
    pushw %si
    pushw %cx

    movl __first_par__, %eax

sleep_again:

    cmpl $0, %eax
    jz sleep_end
    movw MS_1, %cx
    movw $0x80, %dx
    movw $0, %si
    rep outsb
    decl %eax

    jmp sleep_again

sleep_end:
    popw %cx
    popw %si
    popw %dx

    pushl __ret_address__
    ret

#|---------------------------------------|#
#|             u32 _get_cr0()            |#
#|---------------------------------------|#
_get_cr0:
    movl %cr0, %eax
    ret

_get_ds:
    movl $0, %eax
    movw %ds, %ax
    ret

_get_cs:
    movl $0, %eax
    movw %cs, %ax
    ret

_get_ss:
    movl $0, %eax
    movw %ss, %ax
    ret

#|---------------------------------------|#
#|             void _start_up()          |#
#|---------------------------------------|#
_start_up:
	invd
	# Now cache is empty

	movl %cr0, %eax
	orl $0x60000000, %eax
	movl %eax, %cr0
	# Cache is unused
	ret


#|---------------------------------------|#
#|      void _add_new_address(void*)     |#
#|---------------------------------------|#

_add_new_address:
# First par == asked address

	popl __ret_address__
	popl __first_par__
	pushl __ret_address__

	movl %cr0, %eax
	andl $0x9FFFFFFF, %eax
	movl %eax, %cr0
	# Now cache is used

	movb (__first_par__), %al
	
	inc %al

	movb %al, (__first_par__)

	movl %cr0, %eax
	orl $0x60000000, %eax
	movl %eax, %cr0
	# Now cache is unused


	pushl __ret_address__
	ret


_check_mem:
	movb $0, 0xDC
	movl $0, %eax
	movb $0x15, %al

	movb $0xDC, movb_lab + 1

movb_lab:
	movb %al, 0x0
	movb 0xDC, %al
	ret

#|---------------------------------------|#
#|           void _flush_cache()         |#
#|---------------------------------------|#

_flush_cache:
	wbinvd
	ret

_set_real_mode:
	movl %cr0, %eax
	andb $0xFE, %al
	movl %eax, %cr0

        cli
	ret



_in_b:
    popl __ret_address__
    popl __first_par__     #Port
    popl __second_par__    #address in mem

    in __first_par__, (__second_par__)
    pushl __ret_address__
    pushl __ret_address__
    pushl __ret_address__
    ret

#|---------------------------------------|#
#|   time_t* _get_time_access(void*)     |#
#|---------------------------------------|#

time:
time_low:
.long 0x12345678
time_high:
.long 0x9ABCDEF0

_get_time_access:

	popl __ret_address__
	popl __first_par__
	pushl __ret_address__

    pushl %edx
    pushl (__first_par__)

	movw __first_par__, %ax

	#movb %al, ta_movb + 1
	movb %ah, ta_movb + 2

    rdtsc

	movl %eax, time_high
	
	movl %edx, time_low
ta_movb:
	movb %al, 0x0

	rdtsc
	
	subl time_high, %eax

	subl time_low, %edx

	jo __overflow
	jmp __no_overflow
__overflow:
	dec %eax
__no_overflow:
	
	movl %eax, time_high
	
	movl %edx, time_low 

	movl $time, %eax

    popl (__first_par__)
    popl %edx
	pushl __ret_address__
	ret

#|---------------------------------------|#
#|    time_t* _get_time_access_novar()   |#
#|---------------------------------------|#

time_novar:
time_novar_low:
.long 0x12345678
time_novar_high:
.long 0x9ABCDEF0

_get_time_access_novar:

    pushl %edx

	rdtsc

	movl %eax, time_novar_high
	
	movl %edx, time_novar_low

    

next_rdtsc_novar:
	rdtsc
	
	subl time_novar_high, %eax

	subl time_novar_low, %edx

	jo __overflow_novar
	jmp __no_overflow_novar
__overflow_novar:
	dec %eax
__no_overflow_novar:
	
	movl %eax, time_novar_high
	
	movl %edx, time_novar_low 

	movl $time_novar, %eax

    popl %edx
	ret


#|---------------------------------------|#
#|    struct TwoTSC* _get_2_tsc(void*)   |#
#|---------------------------------------|#

struct_tsc:
tsc1_low:
.long 0x0
tsc1_high:
.long 0x0
tsc2_low:
.long 0x0
tsc2_high:
.long 0x0

_get_2_tsc:

    popl __ret_address__
    popl __first_par__
    pushl __ret_address__
    pushl %edx

    rdtsc

    movl %eax, tsc1_high
    movl %edx, tsc1_low

    movb (__first_par__), %al

    rdtsc

    movl %eax, tsc2_high
    movl %edx, tsc2_low

    movl $struct_tsc, %eax

    popl %edx
    pushl __ret_address__
    ret


#|---------------------------------------|#
#|          void _end_of_work()          |#
#|---------------------------------------|#

_end_of_work:
	movl %cr0, %eax
	andl $0x9FFFFFFF, %eax
	movl %eax, %cr0
	ret
