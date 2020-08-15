#ifndef ISR_H
#define ISR_H

typedef struct s_context
{
	unsigned int ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
} context_t;

#endif

