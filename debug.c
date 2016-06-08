#include "includes/debug.h"
#include "includes/console.h"
#include "includes/framebuffer.h"
#include "includes/characters.h"
#include "includes/types.h"
#include "includes/uspios.h"

#define FLOAT_TEXT "Floats unsupported."

void LogPrint(char *buffer, unsigned int ind) {
    unsigned int counter = 0;
    while(counter < ind && counter < (_fbWidth/10)) {
        if(buffer[counter] == '\n') { newLine(&terminal); counter++;}
        else {
            DrawCharacter(getX(terminal), getY(terminal), buffer[counter] - 32);
            terminal.column++;
            counter++;
        }
    }
}

void LogPrintF(char* format, unsigned int formatLength, ...) {
	va_list args;
	char messageBuffer[160];
	unsigned int messageIndex, width = 0, precision = 1, characters;
	int svalue; unsigned int value;
	bool opened, flagged, widthed = false, precisioned = false, length = false;
	bool left = false, plus = false, space = false, hash = false, zero = false, nwidth = false, period = false, nprecision = false;
	bool longInt = false, shortInt = false;
	char character; char* string;
	char base;
	messageIndex = 0;
	opened = false;
	
	va_start(args, formatLength);

	unsigned int index;
	for (index = 0; index < formatLength && messageIndex < sizeof(messageBuffer) - 1; index++) {
		if (opened) {
			if (!flagged)
				switch (format[index]) {
				case '-':
					if (!left) left = true;
					else flagged = true;
					break;
				case '+':
					if (!plus) plus = true;
					else flagged = true;
					break;
				case ' ':
					if (!space) space = true;
					else flagged = true;
					break;
				case '#':
					if (!hash) hash = true;
					else flagged = true;
					break;
				case '0':
					if (!zero) zero = true;
					else flagged = true;
					break;
				default:
					flagged = true;
				}
			if (flagged && !widthed) {
				switch (format[index]) {
				case '0': case '1':
				case '2': case '3':
				case '4': case '5':
				case '6': case '7':
				case '8': case '9':
					nwidth = true;
					width *= 10;
					width += format[index] - '0';
					continue;
				case '*':
					if (!nwidth) {
						widthed = true;
						width = va_arg(args, unsigned int);
						continue;
					}
					else
						widthed = true;
					break;
				default:
					widthed = true;
				}			
			}
			if (flagged && widthed && !precisioned) {
				if (!period) {
					if (format[index] == '.') {
						period = true;
						precision = 0;
						continue;
					} else
						precisioned = true;
				}
				else {
					switch (format[index]) {
					case '0': case '1':
					case '2': case '3':
					case '4': case '5':
					case '6': case '7':
					case '8': case '9':
						nprecision = true;
						precision *= 10;
						precision += format[index] - '0';
						continue;
					case '*':
						if (!nprecision) {
							precisioned = true;
							precision = va_arg(args, unsigned int);
							continue;
						}
						else
							precisioned = true;
						break;
					default:
						precisioned = true;
					}
				}
			}
			if (flagged && widthed && precisioned && !length) {
				switch (format[index]) {
				case 'h':
					length = true;
					shortInt = true;
					continue;
				case 'l':
					length = true;
					longInt = true;
					continue;
				case 'L':
					length = true;
					continue;
				default:
					length = true;
				}
			}
			if (flagged && widthed && precisioned && length) {
				character = '%';
				base = 16;
				switch (format[index]) {
				case 'c':
					character = va_arg(args, int) & 0x7f;
				case '%':
					messageBuffer[messageIndex++] = character;
					break;
				case 'd':
				case 'i':
					if (shortInt) svalue = (int)((short)va_arg(args, int) & 0xffff);
					else if (longInt) svalue = va_arg(args, s64);
					else svalue = va_arg(args, int);

					characters = 1;
					if (svalue < 0) {
						svalue = -svalue;
						messageBuffer[messageIndex++] = '-';
					}
					else if (plus)
						messageBuffer[messageIndex++] = '-';
					else if (space)
						messageBuffer[messageIndex++] = ' ';
					else 
						characters = 0;
		
					unsigned int digits;
					for (digits = 0; digits < precision || svalue > 0; digits++, characters++) {
						unsigned int j;
						for (j = 0; j < digits; j++)
							if (messageIndex - j < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex - j] = messageBuffer[messageIndex - j - 1];
						if (messageIndex - digits < sizeof(messageBuffer) - 1)
							messageBuffer[messageIndex++ -digits] = '0' + (svalue % 10);
						svalue /= 10;
					}		
					
					if (characters < width) {
						if (!left) {
							unsigned int i;
							for (i = 0; i <= characters; i++) {
								if (messageIndex - characters + width - i < sizeof(messageBuffer) - 1)
									messageBuffer[messageIndex - characters + width - i] = 
										messageBuffer[messageIndex - i];
							}
						}
						unsigned int digits;
						for (digits = characters; characters < width; characters++) {
							if (messageIndex - (!left ? digits : 0) < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex - (!left ? digits : 0)] = zero ? '0' : ' '; 
						}
					}
					break;
				case 'e':
				case 'E':
				case 'f':
				case 'g':
				case 'G':
					{
						unsigned int i;
					for (i = 0; (i < width || i < sizeof(FLOAT_TEXT)) && messageIndex < sizeof(messageBuffer) - 1; i++) {
						if (i < sizeof(FLOAT_TEXT))
							messageBuffer[messageIndex++] = FLOAT_TEXT[i];
						else 
							messageBuffer[messageIndex++] = zero ? '0' : ' ';
					}
					}
					break;
				case 'o':
					base = 8;
				case 'u':
					if (format[index] == 'u') base = 10;
				case 'x':
				case 'X':
				case 'p':
					if (shortInt) value = va_arg(args, unsigned int) & 0xffff;
					else if (longInt) value = va_arg(args, u64);
					else value = va_arg(args, unsigned int);

					characters = 1;
					if (plus)
						messageBuffer[messageIndex++] = '-';
					else if (space)
						messageBuffer[messageIndex++] = ' ';
					else 
						characters = 0;

					if (hash) {
						if (format[index] == 'o') {
							if (messageIndex < sizeof(messageBuffer) - 1) 
								messageBuffer[messageIndex++] = '0';
							characters++;
						}
						else if (format[index] != 'u') {
							if (messageIndex < sizeof(messageBuffer) - 1) 
								messageBuffer[messageIndex++] = '0';
							characters++;
							if (messageIndex < sizeof(messageBuffer) - 1) 
								messageBuffer[messageIndex++] = format[index];
							characters++;
						}
					}
					
					for (digits = 0; digits < precision || value > 0; digits++, characters++) {
						unsigned int j;
						for (j = 0; j < digits; j++)
							if (messageIndex - j < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex - j] = messageBuffer[messageIndex - j - 1];
						if (messageIndex - digits < sizeof(messageBuffer) - 1)
							messageBuffer[messageIndex++ -digits] =
							 (value % base) >= 10 ? format[index] - ('X' - 'A') + ((value % base) - 10) : '0' + (value % base);
						value /= base;
					}		

					if (characters < width) {
						if (!left) {
							unsigned int i;
							for (i = 0; i <= characters; i++) {
								if (messageIndex - characters + width - i < sizeof(messageBuffer) - 1)
									messageBuffer[messageIndex - characters + width - i] = 
										messageBuffer[messageIndex - i];
							}
						}

						unsigned int digits;
						for (digits = characters; characters < width; characters++) {
							if (messageIndex - (!left ? digits : 0) < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex++ - (!left ? digits : 0)] = zero ? '0' : ' '; 
						}
					}
					break;
				case 's':
				{
					string = va_arg(args, char*);
					unsigned int i;
					for (i = 0; messageIndex < sizeof(messageBuffer) - 1 && string[i] != '\0' && (!period || i < precision); i++) {
						messageBuffer[messageIndex++] = string[i];
					}
				}
					break;
				case 'n':
					*va_arg(args, unsigned int*) = messageIndex;
					break;
				}
				opened = false;
			}
		} else if (format[index] == '%') {
			opened = true;
			flagged = false;
			widthed = false;
			precisioned = false;
			length = false;
			width = 0; precision = 1;
			left = false; plus = false; space = false; hash = false; zero = false; nwidth = false; period = false; nprecision = false;
			longInt = false; shortInt = false;
		}
		else
			messageBuffer[messageIndex++] = format[index];
	}

	va_end(args);
	
	LogPrint(messageBuffer, messageIndex);
}


//////////////////////////////////////////////////////////////////////////////////////
//										USPIOS									    //
//////////////////////////////////////////////////////////////////////////////////////

void LogWrite(const char *pSource, unsigned int Severity, const char *format, ...) {
	va_list args;
	unsigned int formatLength = strlen(format);
	char messageBuffer[160];
	unsigned int messageIndex, width = 0, precision = 1, characters;
	int svalue; unsigned int value;
	bool opened, flagged, widthed = false, precisioned = false, length = false;
	bool left = false, plus = false, space = false, hash = false, zero = false, nwidth = false, period = false, nprecision = false;
	bool longInt = false, shortInt = false;
	char character; char* string;
	char base;
	messageIndex = 0;
	opened = false;
	
	va_start(args, formatLength);

	unsigned int index;
	for (index = 0; index < formatLength && messageIndex < sizeof(messageBuffer) - 1; index++) {
		if (opened) {
			if (!flagged)
				switch (format[index]) {
				case '-':
					if (!left) left = true;
					else flagged = true;
					break;
				case '+':
					if (!plus) plus = true;
					else flagged = true;
					break;
				case ' ':
					if (!space) space = true;
					else flagged = true;
					break;
				case '#':
					if (!hash) hash = true;
					else flagged = true;
					break;
				case '0':
					if (!zero) zero = true;
					else flagged = true;
					break;
				default:
					flagged = true;
				}
			if (flagged && !widthed) {
				switch (format[index]) {
				case '0': case '1':
				case '2': case '3':
				case '4': case '5':
				case '6': case '7':
				case '8': case '9':
					nwidth = true;
					width *= 10;
					width += format[index] - '0';
					continue;
				case '*':
					if (!nwidth) {
						widthed = true;
						width = va_arg(args, unsigned int);
						continue;
					}
					else
						widthed = true;
					break;
				default:
					widthed = true;
				}			
			}
			if (flagged && widthed && !precisioned) {
				if (!period) {
					if (format[index] == '.') {
						period = true;
						precision = 0;
						continue;
					} else
						precisioned = true;
				}
				else {
					switch (format[index]) {
					case '0': case '1':
					case '2': case '3':
					case '4': case '5':
					case '6': case '7':
					case '8': case '9':
						nprecision = true;
						precision *= 10;
						precision += format[index] - '0';
						continue;
					case '*':
						if (!nprecision) {
							precisioned = true;
							precision = va_arg(args, unsigned int);
							continue;
						}
						else
							precisioned = true;
						break;
					default:
						precisioned = true;
					}
				}
			}
			if (flagged && widthed && precisioned && !length) {
				switch (format[index]) {
				case 'h':
					length = true;
					shortInt = true;
					continue;
				case 'l':
					length = true;
					longInt = true;
					continue;
				case 'L':
					length = true;
					continue;
				default:
					length = true;
				}
			}
			if (flagged && widthed && precisioned && length) {
				character = '%';
				base = 16;
				switch (format[index]) {
				case 'c':
					character = va_arg(args, int) & 0x7f;
				case '%':
					messageBuffer[messageIndex++] = character;
					break;
				case 'd':
				case 'i':
					if (shortInt) svalue = (int)((short)va_arg(args, int) & 0xffff);
					else if (longInt) svalue = va_arg(args, s64);
					else svalue = va_arg(args, int);

					characters = 1;
					if (svalue < 0) {
						svalue = -svalue;
						messageBuffer[messageIndex++] = '-';
					}
					else if (plus)
						messageBuffer[messageIndex++] = '-';
					else if (space)
						messageBuffer[messageIndex++] = ' ';
					else 
						characters = 0;
		
					unsigned int digits;
					for (digits = 0; digits < precision || svalue > 0; digits++, characters++) {
						unsigned int j;
						for (j = 0; j < digits; j++)
							if (messageIndex - j < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex - j] = messageBuffer[messageIndex - j - 1];
						if (messageIndex - digits < sizeof(messageBuffer) - 1)
							messageBuffer[messageIndex++ -digits] = '0' + (svalue % 10);
						svalue /= 10;
					}		
					
					if (characters < width) {
						if (!left) {
							unsigned int i;
							for (i = 0; i <= characters; i++) {
								if (messageIndex - characters + width - i < sizeof(messageBuffer) - 1)
									messageBuffer[messageIndex - characters + width - i] = 
										messageBuffer[messageIndex - i];
							}
						}
						unsigned int digits;
						for (digits = characters; characters < width; characters++) {
							if (messageIndex - (!left ? digits : 0) < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex - (!left ? digits : 0)] = zero ? '0' : ' '; 
						}
					}
					break;
				case 'e':
				case 'E':
				case 'f':
				case 'g':
				case 'G':
					{
						unsigned int i;
					for (i = 0; (i < width || i < sizeof(FLOAT_TEXT)) && messageIndex < sizeof(messageBuffer) - 1; i++) {
						if (i < sizeof(FLOAT_TEXT))
							messageBuffer[messageIndex++] = FLOAT_TEXT[i];
						else 
							messageBuffer[messageIndex++] = zero ? '0' : ' ';
					}
					}
					break;
				case 'o':
					base = 8;
				case 'u':
					if (format[index] == 'u') base = 10;
				case 'x':
				case 'X':
				case 'p':
					if (shortInt) value = va_arg(args, unsigned int) & 0xffff;
					else if (longInt) value = va_arg(args, u64);
					else value = va_arg(args, unsigned int);

					characters = 1;
					if (plus)
						messageBuffer[messageIndex++] = '-';
					else if (space)
						messageBuffer[messageIndex++] = ' ';
					else 
						characters = 0;

					if (hash) {
						if (format[index] == 'o') {
							if (messageIndex < sizeof(messageBuffer) - 1) 
								messageBuffer[messageIndex++] = '0';
							characters++;
						}
						else if (format[index] != 'u') {
							if (messageIndex < sizeof(messageBuffer) - 1) 
								messageBuffer[messageIndex++] = '0';
							characters++;
							if (messageIndex < sizeof(messageBuffer) - 1) 
								messageBuffer[messageIndex++] = format[index];
							characters++;
						}
					}
					
					for (digits = 0; digits < precision || value > 0; digits++, characters++) {
						unsigned int j;
						for (j = 0; j < digits; j++)
							if (messageIndex - j < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex - j] = messageBuffer[messageIndex - j - 1];
						if (messageIndex - digits < sizeof(messageBuffer) - 1)
							messageBuffer[messageIndex++ -digits] =
							 (value % base) >= 10 ? format[index] - ('X' - 'A') + ((value % base) - 10) : '0' + (value % base);
						value /= base;
					}		

					if (characters < width) {
						if (!left) {
							unsigned int i;
							for (i = 0; i <= characters; i++) {
								if (messageIndex - characters + width - i < sizeof(messageBuffer) - 1)
									messageBuffer[messageIndex - characters + width - i] = 
										messageBuffer[messageIndex - i];
							}
						}

						unsigned int digits;
						for (digits = characters; characters < width; characters++) {
							if (messageIndex - (!left ? digits : 0) < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex++ - (!left ? digits : 0)] = zero ? '0' : ' '; 
						}
					}
					break;
				case 's':
				{
					string = va_arg(args, char*);
					unsigned int i;
					for (i = 0; messageIndex < sizeof(messageBuffer) - 1 && string[i] != '\0' && (!period || i < precision); i++) {
						messageBuffer[messageIndex++] = string[i];
					}
				}
					break;
				case 'n':
					*va_arg(args, unsigned int*) = messageIndex;
					break;
				}
				opened = false;
			}
		} else if (format[index] == '%') {
			opened = true;
			flagged = false;
			widthed = false;
			precisioned = false;
			length = false;
			width = 0; precision = 1;
			left = false; plus = false; space = false; hash = false; zero = false; nwidth = false; period = false; nprecision = false;
			longInt = false; shortInt = false;
		}
		else
			messageBuffer[messageIndex++] = format[index];
	}

	va_end(args);
	
	switch(Severity) {
		case LOG_ERROR:
			setForeColor(0x0000FFFF);
			break;
		case LOG_NOTICE:
			setForeColor(0xFF0000FF);
			break;
		case LOG_WARNING:
			setForeColor(0x00FF00FF);
			break;
		case LOG_DEBUG:
			break;
		default:
			break;
	}
	
	LogPrint(messageBuffer, messageIndex);
	setForeColor(0xFFFFFFFF);
}

void uspi_assertion_failed (const char *pExpr, const char *pFile, unsigned nLine) {
	strcat(pExpr, pFile);
	strcat(pExpr, " %d");
	LogWrite("", LOG_ERROR, pExpr, nLine);
	
	while(1);
}

void DebugHexdump (const void *pBuffer, unsigned nBufLen, const char *pSource /* = 0 */) {
	LOGF(pBuffer);
}