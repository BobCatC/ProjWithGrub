.text

.global _start_up
.global _flush_cache
.global _add_new_address
.global _end_of_work

#|---------------------------------------|#
#|			 void func_name(par)		 |#
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
#|			 void _start_up()		     |#
#|---------------------------------------|#
_start_up:
	invd
	# Now cache is empty

	movl %cr0, %eax
	orl $0x60000000, %eax
	movl %eax, %cr0
	# Cache is unused
	ret
#				 -------				 #

#|---------------------------------------|#
#|	  void _add_new_address(void*)	     |#
#|---------------------------------------|#

.comm add_new_address_ret, 4
.comm add_new_address_first_par, 4

_add_new_address:
# First par == asked address
	popl add_new_address_ret
	popl add_new_address_first_par
	pushl add_new_address_ret

	movl %cr0, %eax
	andl $0x9FFFFFFF, %eax
	movl %eax, %cr0
	# Now cache is used

	movb (add_new_address_first_par), %al
	
	inc %al

	movb %al, (add_new_address_first_par)

	movl %cr0, %eax
	orl $0x60000000, %eax
	movl %eax, %cr0
	# Now cache is unused


	pushl add_new_address_ret
	ret
#				 -------				 #

#|---------------------------------------|#
#|		   void _flush_cache()		     |#
#|---------------------------------------|#

_flush_cache:
	wbinvd
	ret
#				 -------				 #


#|---------------------------------------|#
#|		  void _end_of_work()		     |#
#|---------------------------------------|#

_end_of_work:
	movl %cr0, %eax
	andl $0x9FFFFFFF, %eax
	movl %eax, %cr0
	ret
#				 -------				 #
