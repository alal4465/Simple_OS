#include "../drivers/screen.h"

void main(){

	screen_clear();

	char *kernel_message = "Started the kernel successfully!\n";

	kprint(kernel_message);

}
