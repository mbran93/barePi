#ifndef __CONSOLE_H__
#define __CONSOLE_H__

volatile unsigned int *fb;

typedef struct {
    unsigned int row;
    unsigned int column;
} console;

console terminal;

void consoleInit();
void newLine(console *);
void scroll();
unsigned int getX(console);
unsigned int getY(console);

#endif