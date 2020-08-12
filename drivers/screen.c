#include "io.h"
#include "screen.h"

#define COLOR_WHITE  0x07

#define VGA_REG_DATA 0x3d5
#define VGA_REG_CTRL 0x3d4

unsigned char* video_mem = (unsigned char*)0xb8000;
int row = 0;
int col = 0;

void kprint(char* str){
	for(int i = 0; str[i] != '\0'; i++)
		kprint_c(str[i]);
}

void kprint_c(char ch){

	if(ch >= 0x20 && ch < 0x7f) {
		video_mem[(row * 80 + col) * 2] = ch;
		video_mem[(row * 80 + col) * 2 + 1] = COLOR_WHITE;

		col++;

		if(col == 80) {
			col = 0;
			row++;
		}
	} else {
		switch(ch) {
			case 0x0a: // '\n'
				col = 0;
				row++;				

				break;

			case 0x0d: // '\r'
				col = 0;
				break;
			
			case 0x09: // '\t'
				col += 4;

				if(col >= 80) {
					col = col % 80;
					row++;
				}

				break;

			case 0x08: // '\b'
				if(col > 0) {
					col--;
				} else if(row > 0) {
					col = 79;
					row--;
				}

				video_mem[(row * 80 + col) * 2] = ' ';

				break;
		}
	}

	if(row == 25) {
		screen_scroll();

		row--;
		col = 0;
	}

	cursor_update();

}

void screen_clear(){
	
	for(int i = 0; i < 80 * 25 * 2; i += 2) {
		video_mem[i] = ' ';
		video_mem[i + 1] = COLOR_WHITE;
	}

	row = 0;
	col = 0;

	cursor_update();

}

void screen_scroll(){

	for(int i = 0; i < 80 * 24 * 2; i++)
		video_mem[i] = video_mem[i + 160];

	for(int i = 80 * 24 * 2; i < 80 * 25 * 2; i += 2) {
		video_mem[i] = ' ';
		video_mem[i + 1] = COLOR_WHITE;
	}

}

void cursor_update(){
	unsigned short offset = row * 80 + col;

	byte_out(VGA_REG_CTRL, 0x0f);
	byte_out(VGA_REG_DATA, (unsigned char) (offset & 0xff));

	byte_out(VGA_REG_CTRL, 0x0e);
	byte_out(VGA_REG_DATA, (unsigned char) (offset >> 8));
}
