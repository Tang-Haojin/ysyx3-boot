.option norvc
.section .text
.extern init
.global _start
_start:
	li sp,0x90000000
	jal init
