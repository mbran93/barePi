#include "includes/helper.h"

// void memcpy(void *dest, void *src, unsigned int n) {
// 	char *csrc = ( char * )src;
// 	char *cdest = ( char * )dest;

// 	unsigned int i = 0;
// 	for( ; i < n; i++ ) {
// 		cdest[i] = csrc[i];//	DrawCharacter(10, 10, 'A' - 32, 0xFFFFFFFF);
// 	}
// }

// void  *memset(void *b, unsigned int c, unsigned int len) {
//   unsigned char *p = b;
//   while(len > 0)
//     {
//       *p = c;
//       p++;
//       len--;
//     }
//   return(b);
// }

// void itoa(unsigned int num, char *buffer) {
// 	unsigned int count = 0;

// 	while(num > 0) {
// 		buffer[count] = (num % 10) + 48;
// 		num /= 10;
// 		count++;
// 	}

// 	unsigned int temp, length = count;
// 	count = 0;

// 	for(; count < length/2; count++) {
// 		temp = buffer[length - count - 1];
// 		buffer[length - count - 1] = buffer[count];
// 		buffer[count] = temp;
// 	}

// 	buffer[length] = '\0';
// }

// unsigned int strlen(char *str) {
// 	unsigned int counter = 0;

// 	while(str[counter] != '\0') {
// 		counter++;
// 	}

// 	return counter;
// }

// unsigned int strcat(char *str1, char *str2, char *dest) {
// 	unsigned int ind = 0;
// 	if(sizeof(dest) > (strlen(str1) + strlen(str2))) {
// 		for(; ind < strlen(str1); ind++) {
// 			dest[ind] = str1[ind];
// 		}
// 		for(; ind < (strlen(str1) + strlen(str2)); ind++) {
// 			dest[ind] = str2[ind];
// 		}
// 	}
// 	return ind;
// }