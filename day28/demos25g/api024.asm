GLOBAL api_fread

SECTION .text

api_fsize:			; int api_fsize(int fhandle, int mode);
		MOV		EDX,24
		MOV		EAX,[ESP+4]			; fhandle
		MOV		ECX,[ESP+8]			; mode
		INT		0x40
		RET
