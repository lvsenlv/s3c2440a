.text
.global _start

_start :
	ldr 	sp, 			=4*1024
	bl 		system_init
	bl		main

halt_loop :
    b 		halt_loop
    