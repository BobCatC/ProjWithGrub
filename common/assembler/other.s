.text

.global _check_mem
.global _Sleep
.global _set_video_mode

.global _set_real_mode

#|---------------------------------------|#
#|			 void func_name(par)	     |#
#|---------------------------------------|#
#DATA:
.comm func_proto_ret, 4
.comm func_proto_first_par, 4
#CODE:
_func_proto:
	popl func_proto_ret


	pushl func_proto_ret
	ret
#				 -------				 #

#|---------------------------------------|#
#|			 void _Sleep(u32 nMS)	     |#
#|---------------------------------------|#

.comm sleep_ret, 4
.comm sleep_first_par, 4

MS_1:
.short 1000

_Sleep:

	popl sleep_ret
	popl sleep_first_par
	pushl sleep_ret

	pushw %dx
	pushw %si
	pushw %cx

	movl sleep_first_par, %eax

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

	pushl sleep_ret
	ret
#				 -------				 #

#|---------------------------------------|#
#|			void _set_video_mode	     |#
#|---------------------------------------|#

#CODE:
_set_video_mode:
	movb $0, %ah
	movb $3, %al

	int $0x10

	ret
#				 -------				 #

#|---------------------------------------|#
#|			 BYTE _check_mem()		     |#
#|---------------------------------------|#
_check_mem:
	movb $0, 0xDC
	movl $0, %eax
	movb $0x15, %al

	movb $0xDC, movb_lab + 1

movb_lab:
	movb %al, 0x0
	movb 0xDC, %al
	ret
#				 -------				 #

#|---------------------------------------|#
#|		  void _set_real_mode()		     |#
#|---------------------------------------|#
_set_real_mode:
	movl %cr0, %eax
	andb $0xFE, %al
	movl %eax, %cr0

	cli
	ret
#				 -------				 #















