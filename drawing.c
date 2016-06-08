#include "includes/drawing.h"
#include "includes/framebuffer.h"

void DrawPixel(unsigned int x, unsigned int y) {
    volatile unsigned int *rb = (unsigned int *)framebuffer->ptr;
    
    volatile unsigned int count = x + _fbWidth * y;
    rb[count] = foreColor;
}

void setForeColor(unsigned int color) {
    foreColor = color;
}

void setBackColor(unsigned int color) {
    backColor = color;   
}