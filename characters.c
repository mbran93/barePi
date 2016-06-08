#include "includes/characters.h"
#include "includes/drawing.h"
#include "includes/teletext.h"

void DrawCharacter(unsigned int x, unsigned int y, unsigned int ASCII) {
    unsigned int row = 0, column;
    unsigned int ander;
    
    for(; row < 9; row++) {
        ander = 16;
        column = 0;
        for(; column < 5; column++) {
            if((ander & teletext[ASCII][row]) > 0) DrawPixel(x + column, y + row);
            ander = ander >> 1;
        }
    }
}

void DrawString(unsigned int x, unsigned int y, char *msg) {
    unsigned int ind = 0;
    while(msg[ind] != '\0') {
        DrawCharacter(x, y, msg[ind] - 32);
        x += 10;
        ind++;
    }
}