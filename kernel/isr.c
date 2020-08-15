#include "isr.h"
#include "../drivers/screen.h"

void isr_handler(context_t regs)
{
	kprint("Recieved interrupt!\n");
	
	char* msg_arr[256];
	for(int i = 0 ; i < 256 ; i++)
		msg_arr[i] = "Reserved\n";

	msg_arr[0] = "0x00\n";
	msg_arr[0x80] = "0x80\n";

	kprint(msg_arr[regs.int_no]);
}

