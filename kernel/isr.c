#include "isr.h"

void (*isr_callbacks[16])();
interrupt_handler interrupt_handlers[256];

void register_isr_callback(int irq,void (*callback)()){
	isr_callbacks[irq-IRQ0]=callback;
}

void register_interrupt_handler(unsigned int index, interrupt_handler handler){
	interrupt_handlers[index] = handler;
}

void isr_handler(context_t regs)
{
	kprint("Recieved interrupt!\n");
	
	char* msg_arr[256];
	for(int i = 0 ; i < 256 ; i++)
		msg_arr[i] = "Reserved\n";

	msg_arr[0] = "0x00\n";
	msg_arr[0x80] = "0x80\n";

	kprint(msg_arr[regs.int_no]);

	if (interrupt_handlers[regs.int_no] != 0)
		interrupt_handlers[regs.int_no](regs);
}

void irq_handler(context_t regs)
{
	byte_out(0x20, 0x20);
	if (regs.int_no >= IRQ8){
		byte_out(0xA0, 0x20);
	}
	if (isr_callbacks[regs.int_no-IRQ0]!=0){
		(*isr_callbacks[regs.int_no-IRQ0])();
	}
	else{
		kprint("Recieved IRQ\n");
	}
}
