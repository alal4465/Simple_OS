#ifndef SCREEN_H
#define SCREEN_H

void cursor_update();
void screen_scroll();
void screen_clear();

void kprint_c(char ch);
void kprint(char* str);

#endif /* SCREEN_H  */
