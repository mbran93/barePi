#include "includes/console.h"
#include "includes/framebuffer.h"
#include "includes/helper.h"
#include "includes/types.h"
#include "string.h"

void consoleInit() {
    if(framebuffer_init != 1) InitialiseFrameBuffer(480, 360, 32);
    fb = getFrameBuffer()->ptr;
    
    terminal.row = 0;
    terminal.column = 0;
}

unsigned int getX(console con) {
    return (con.column) * 10;
}

unsigned int getY(console con) {
    return (con.row) * 12;
}

void newLine(console *con) {
    con->row++;
    con->column = 0;
    
    // if(con->row >= (_fbHeight / 12)) scroll();
}

void scroll() {
    // memcpy(getFrameBuffer()->ptr, (getFrameBuffer()->ptr + _fbWidth), _fbWidth * _fbHeight);
}