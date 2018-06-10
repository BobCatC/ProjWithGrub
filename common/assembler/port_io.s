.text

.global _in_b_port_60
.global _in_b_port_64
.global _out_b

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
#|		BYTE _in_b(BYTE iPort)		     |#
#|---------------------------------------|#

#CODE:
_in_b_port_60:
	movl $0, %eax

	inb $0x60, %al
	ret
#				 -------				 #

_in_b_port_64:
	movl $0, %eax

	inb $0x64, %al
	ret

#|---------------------------------------|#
#|	void _out_b(BYTE iPort, BYTE Data)   |#
#|---------------------------------------|#
#DATA:
.comm out_b_ret, 4
.comm out_b_first_par, 4
.comm out_b_second_par, 4
#CODE:
_out_b:
	popl out_b_ret
	popl out_b_first_par	#iPort
	popl out_b_second_par   #Data
	pushl out_b_ret
	pushl out_b_ret

	movl $0, %eax

	movb out_b_first_par, %al
	movb out_b_second_par, %ah

	#out %al, %ah

	pushl out_b_ret
	ret
#				 -------				 #
