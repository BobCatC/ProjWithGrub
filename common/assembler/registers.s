.text

.global _get_cr0
.global _get_ds
.global _get_cs
.global _get_ss

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
#|			 u32 _get_cr0()			     |#
#|---------------------------------------|#
_get_cr0:
	movl %cr0, %eax
	ret
#				 -------				 #

#|---------------------------------------|#
#|			 u32 _get_ds()			     |#
#|---------------------------------------|#
_get_ds:
	movl $0, %eax
	movw %ds, %ax
	ret
#				 -------				 #

#|---------------------------------------|#
#|			 u32 _get_cs()			     |#
#|---------------------------------------|#
_get_cs:
	movl $0, %eax
	movw %cs, %ax
	ret
#				 -------				 #

#|---------------------------------------|#
#|			 u32 _get_ss()			     |#
#|---------------------------------------|#
_get_ss:
	movl $0, %eax
	movw %ss, %ax
	ret
#				 -------				 #

